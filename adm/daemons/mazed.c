// Written by Lonely@nitan3

#include <ansi.h>
inherit F_DBASE;


#define MAZE            "maze.c"
#define ENTRY           "enter.c"
#define EXIT            "out.c"
#define MAZE_DIR        "/d/maze/"
#define MAZE_DATA       "/data/maze/"  

#define DEST_TIME       1800   // 60 分钟后销毁
#define REMOVE_TIME     15     // 30 秒后销毁

int clean_up() { return 1; }

nosave object *total = ({ });
public object *query_total() { return total; }
public int remove_maze(object me, string arg);
private void init_player(object me);
private void restore_status(object me);
private void auto_check();
int check_out(object me);
int check_quit(object me);

public string query_maze_file(string x, string y)
{
        return sprintf(MAZE_DATA + "%s/%s/", x, y);
}

void create()
{
        seteuid(ROOT_UID);
        set("channel_id", "迷宫精灵");
        CHANNEL_D->do_channel(this_object(), "sys", "迷宫系统已经启动。");
}

protected mixed create_maze(object me, string maze)
{
        string src, dst;
        string filename;

        src = MAZE_DIR + maze + "/" + MAZE;
        dst = query_maze_file(me->query("id"), maze) + MAZE;
        assure_file(dst);
        cp(src, dst);
        
        src = MAZE_DIR + maze + "/" + ENTRY;
        dst = query_maze_file(me->query("id"), maze) + ENTRY;
        filename = dst;
        cp(src, dst);
        
        src = MAZE_DIR + maze + "/" + EXIT;
        dst = query_maze_file(me->query("id"), maze) + EXIT;
        cp(src, dst);
        
        return filename;
}

public int enter_maze(object me, string arg) 
{
        string efile, mfile;
        object *member, ob;
        object *inv, obj;
        string id;
        int s;
        
        if( me->query_condition("killer") )
        {
                tell_object(me, "杀人通缉犯是不能进入副本的。\n");
                return 0;
        }

        if( !wizardp(me) && 
            time() - me->query_temp("maze/time") < 180 )
        {
                tell_object(me, "为了降低游戏CPU负担，游戏副本的创建必须每隔3分钟一次。\n");
                return 0;
        }

        inv = deep_inventory(me);
        if( inv && sizeof(inv) > 0 )
        {
                foreach( obj in inv ) 
                        if( obj && obj->is_character() ) {
                                tell_object(me, "不可以将其他角色或者NPC通过背的方式带进副本内。\n");
                                return 0;
                        }
        }
                
        member = me->query_team_member(); 
        if( !member || !sizeof(member) )
                id = me->query("id");
        else        
                id = me->query_team_leader()->query("id");
                
        efile = query_maze_file(id, arg) + ENTRY;
        mfile = query_maze_file(id, arg) + MAZE; 
        
        // 已经创建过
        if( file_size(efile) > 0 )
        {
                if( !ob = find_object(efile) )
                {                        
                        if( sizeof(member) && ! me->is_team_leader() )
                        { 
                                tell_object(me, "必须由队长来创建副本任务的。\n");
                                return 0;
                        }
                                        
                        if( !ob = load_object(efile) )
                        {
                                tell_object(me, "创建副本任务失败，请告之天神。\n");
                                return 0;
                        }
                        
                        obj = get_object(mfile);
                        obj->set("quest/time", time());
                        
                        s = SCHEDULE_D->set_event(DEST_TIME, 0, this_object(), "remove_maze", me, arg);
                        obj->set("quest/schedule", s);
                }
        } else 
        {       
                // 没有创建过      
                efile = create_maze(me, arg);  
                if( !ob = get_object(efile) )
                {
                        tell_object(me, "创建副本任务失败，请告之天神。\n");
                        return 0;
                }
                
                mfile = query_maze_file(me->query("id"), arg) + MAZE; 
                obj = get_object(mfile);
                obj->set("quest/time", time());
                
                s = SCHEDULE_D->set_event(DEST_TIME, 0, this_object(), "remove_maze", me, arg);
                obj->set("quest/schedule", s);
        }

        if( !arrayp(total) )
                total = ({ me });
        else
        if( member_array(me, total) == -1 )
                total += ({ me });
                        
        tell_object(me, HIR "祝你好运气！\n" NOR);
        me->set_temp("maze/time", time());
        me->set_temp("maze/name", arg);
        me->set_temp("maze/entry_room", environment(me));
        init_player(me);
        me->move(ob); 
        set_heart_beat(1);
        return 1;
}

private void heart_beat()
{
        if( sizeof(total) )
                auto_check();
        else
                set_heart_beat(0);
}

// check all the players who join the maze
private void auto_check()
{
        object ob;
        string room;
        object *lost;
        int i;

        lost = ({ });
        for( i = 0; i < sizeof(total); i++ )
        {
                if( !objectp(total[i]) )
                {
                        total[i] = 0;
                        continue;
                }
                room = base_name(environment(total[i]));
                if( !sscanf(room, MAZE_DATA + "%*s") )
                {
                        // not in maze room
                        restore_status(total[i]);
                        total[i] = 0;
                        continue;
                }
                if( total[i]->query("qi") < 1 || total[i]->query("jing") < 1 )
                        lost += ({ total[i] });
        }

        // kickout the players who lost competition
        foreach( ob in lost )
                check_out(ob);

        total -= ({ 0 });
        return;
}

// set player's override functions
private void init_player(object me)
{
        me->set_override("unconcious", (: call_other, __FILE__, "check_out" :));
        me->set_override("die", (: call_other, __FILE__, "check_out" :));
        // me->set_override("quit", (: call_other, __FILE__, "check_quit" :));
        me->set_temp("backup/killer", me->query_killer());
        me->set_temp("backup/want", me->query_want());

        me->set("backup/condition", me->query_condition());
        me->clear_condition();
        
}

int check_out(object me)
{
        object ob;
        mapping my;
        string msg;
        string room;

        room = base_name(environment(me));
        if( !sscanf(room,  MAZE_DATA +"%*s") )
        {
                // not in maze room
                total -= ({ me });
                return 0;
        }

        my = me->query_entire_dbase();
        my["eff_qi"] = my["max_qi"];
        my["eff_jing"] = my["max_jing"];
        my["qi"] = 1;
        my["jing"] = 1;

        tell_object(me, HIR "\n你觉得眼前一阵模糊...这下完了！\n" NOR);
        if( ob = me->query_last_damage_from() )
                msg = WHT "\n$N被" + ob->name(1) + "打成重伤，狼狈而逃！\n\n" NOR;
        else
                msg = WHT "\n$N身受重伤，狼狈而逃！\n\n" NOR;
                
        message_vision(msg, me);

        restore_status(me);
        total -= ({ me });
        me->move(me->query_temp("maze/entry_room"));
        message("vision", "一个黑影倏的窜了出来，随即就是“啪”的"
                "一声，就见" + me->name() +"摔倒了地上，一副半死不"
                "活的样子。\n", environment(me), ({ me }));
        tell_object(me, "半昏半迷中，你觉得被人拎了起来，又"
                    "重重的摔倒了地上。\n");
        if( !living(me) )
                me->revive();
                
        return 1;
}

// overide function of quit
int check_quit(object me)
{
        restore_status(me);
        if( arrayp(total) )
                total -= ({ me });
                
        me->dismiss_team(me);
        tell_object(me, "你逃出了副本。\n");

        // continue run quit function
        me->move(me->query_temp("maze/entry_room"));
        message("vision", "只见" + me->name() + "慌里慌张的跑了出来。\n",
                environment(me), ({ me }));
        return 1;
}

private void restore_status(object me)
{
        object *inv, obj; 
        mapping cnd;
        string *ks;
        int i;

        inv = deep_inventory(me);
        if( inv && sizeof(inv) > 0 )
        {
                foreach( obj in inv ) 
                        if( obj && obj->query("maze_item") ) 
                                destruct(obj); 
        }
        me->delete_override("unconcious");
        me->delete_override("die");
        // me->delete_override("quit");
        remove_maze(me, me->query_temp("maze/name"));
        
        me->remove_all_enemy(1);
        me->remove_all_killer();
        me->clear_condition();
        me->receive_damage("qi", 0);
        me->apply_killer(me->query_temp("backup/killer"));
        me->apply_want(me->query_temp("backup/want"));

        cnd = me->query("backup/condition");
        me->delete("backup/condition");
        me->delete_temp("block_msg/all");
        if( !mapp(cnd) ) return;
        ks = keys(cnd);
        for( i = 0; i < sizeof(ks); i++ )
                me->apply_condition(ks[i], cnd[ks[i]]);
}

public mixed query_maze_dir(object ob) 
{
        object env;
        string user, maze, dir;
        
        env = environment(ob);
        if( !env ) return 0;

        if( sscanf(base_name(env), MAZE_DATA + "%s/%s/%*s", 
                user, maze) != 3 )
                return 0;
                
        dir = query_maze_file(user, maze);
        
        return dir;
}

public object query_maze_mainobj(object ob) 
{
        string file;
        
        file = query_maze_dir(ob) + MAZE;
        
        return get_object(file);
}

public void delay_remove_maze(object who, string arg) 
{
        SCHEDULE_D->set_event(REMOVE_TIME, 0, this_object(), "remove_maze", who, arg);
        // who->start_call_out((: call_other, __FILE__, 
        //                        "remove_maze", who, arg :), REMOVE_TIME); 
}

public int remove_maze(object me, string arg)
{
        string filename;
        object ob;
        int s;
        
        if( !objectp(me) ) return 0;
        
        me->set_temp("maze/time", time());
        
        filename = query_maze_file(me->query("id"), arg) + MAZE;
        ob = find_object(filename);
        if( ob ) 
        {
                if( s = ob->query("quest/schedule") )
                        SCHEDULE_D->delete_event(s);
                        
                destruct(ob);
        }

        filename = query_maze_file(me->query("id"), arg) + ENTRY;
        ob = find_object(filename);
        if( ob ) destruct(ob);
                
        filename = query_maze_file(me->query("id"), arg) + EXIT;
        ob = find_object(filename);
        if( ob ) destruct(ob);  
          
        return 1;
}
