// ultraquestd.c
// Written by Vin for Heros.cn 2002/08/16.
// Updated by Lonely for nitan.nt
            
#include <ansi.h> 
#include <quest.h> 
 
inherit F_DBASE; 

// 定义提供给外部调用的接口函数 
public mixed accept_ask(object me, object who, string topic); 
public mixed ask_information(object me, object who, string topic); 
public void generate_information(object me); 
public void give_quest(object me); 
public void give_accede(object me, object who); 
public void quest_guard(object me, object who); 
public void quest_going(object me, object who); 
public void give_gift(object me); 
void start_guard(object me, object who); 
void check_guard(object me, object who, object killer); 
void check_going(object me, object who); 
   
#define QUEST           "/adm/etc/questlist/" 
#define OBJ_DIR         "/quest/questobj/" 
#define NPC_DIR         "/quest/questnpc/"
#define QUESTER "南贤" 
#define LURCHER "北丑" 
 
private void special_bonus(object me) 
{ 
        object ob; 
        string un; 

        // 给予随机的物品作奖励 
        string *ob_list = ({ 
                "/clone/fam/etc/hanjing", 
                "/clone/fam/etc/mozhixin",
                "/clone/fam/item/bless_water",
                "/clone/fam/etc/huojingling",
                "/clone/fam/etc/leishentong",
                "/clone/fam/max/zhenyu",
                "/clone/fam/max/longjia",
                "/clone/fam/max/xuanhuang",
                "/clone/fam/pill/dimai",
                "/clone/fam/pill/yulu",
                "/clone/fam/pill/renshen4",
                "/clone/fam/pill/lingzhi4",
                "/clone/fam/pill/xuelian4",
                "/clone/fam/pill/renshen3",
                "/clone/fam/pill/lingzhi3",
                "/clone/fam/pill/xuelian3",
                "/clone/fam/pill/linghui2",
                "/clone/fam/pill/neili2",
                "/clone/fam/pill/puti4",
                "/clone/fam/pill/sheli4",
                "/clone/fam/pill/sheli3",
                "/clone/fam/pill/puti3",
                "/clone/fam/pill/puti2",
                "/clone/fam/pill/sheli2",        
                "/clone/fam/max/xuanhuang",
        }); 
        
        ob = new(ob_list[random(sizeof(ob_list))]); 
        
        // 查看物品的单位属性 
        if (ob->query("base_unit")) 
                un = ob->query("base_unit"); 
        else 
                un = ob->query("unit"); 
                               
        // 保持与正常的 give 信息一致 
        message_vision("说完拿出一" + 
                       un + ob->name() + NOR "(" + ob->query("id") + 
                       ")给$N" NOR "。\n\n" NOR, me); 
        
        ob->move(me, 1); 
} 
        
// 任务的起始程序，给予出最初任务和提供新线索 
public mixed accept_ask(object me, object who, string topic) 
{
        object item, obj; 
        string msg, file, name, id; 
        mapping q; 
        string *ob;

        q = who->query("ultraquest/");        
        if (topic == "任务" || topic == "quest") 
        {
                message_vision("$n向$N打听有关宗师任务的情况。\n", me, who);
                /*
                if (! ultrap(who)) 
                {
                        message_vision("$N瞪大眼睛看着$n，道：“你还没有成为武学大宗师，还帮不了"
                                       "我什么忙！”\n", me, who);
                        return 1;
                }
                */
                                                        
                if (mapp(q)) 
                {
                        switch (q["type"]) 
                        { 
                        case "find":   // 寻物
                                message_vision(CYN "$N" CYN "一脸怒容对$n" CYN "道：我不是让" 
                                               "你把" NOR + HIR + q["name"] + "(" + q["id"] + 
                                               ")" + NOR + CYN "给我找来吗，现在这个东西呢？\n" NOR, 
                                               me, who);   
                                break;
                        case "send":   // 送物
                                message_vision(CYN "$N" CYN "一脸怒容对$n" CYN "道：我不是让"
                                               "你把包裹送给" NOR + HIR + q["name"] + "(" + q["id"] + 
                                               ")" + NOR + CYN "的吗？现在回执呢？\n" NOR, 
                                               me, who);   
                                break;
                        case "rescue": // 救人
                                message_vision(CYN "$N" CYN "一脸怒容对$n" CYN "道：我不是让"
                                               "你去把" NOR + HIR + q["name"] + "(" + q["id"] + 
                                               ")" + NOR + CYN "解救出来吗？他的信物呢？\n" NOR, 
                                               me, who);                                   
                                break;
                                  
                        case "kill":   // 杀人
                                message_vision(CYN "$N" CYN "一脸怒容对$n" CYN "道：我不是让" 
                                               "你去查掀起江湖腥风血雨的幕后主谋了吗？现在他"
                                               "的人头呢？\n" NOR, me, who);
                                break;
        
                        } 
                        return 1;
                }                                             
                
                if (who->query("combat_exp") < 20000000) 
                { 
                        message_vision(CYN "$N" CYN "叹了一口气，对$n" CYN "道：就你那点水平，" 
                                       "我实在没法给你派任务。\n" NOR, me, who); 
                        return 1; 
                } 

                if (who->query("score") < 200000) 
                {
                        message_vision(CYN "$N" CYN "叹了一口气，对$n" CYN "道：你这人阅历太低，"
                                       "不通世道，帮不了我什么忙。\n" NOR, me, who); 
                        return 1;                 
                }

                if (who->query("weiwang") < 200) 
                { 
                        message_sort(CYN "$N" CYN "看了看$n" CYN "，叹气道：你在" 
                                     "江湖上一点名头都没有，我没法放心把任务交给" 
                                     "你。\n" NOR, me, who); 
                        tell_object(who, HIY "看来还是的加强自己的江湖威望才行。\n" NOR); 
                        return 1; 
                } 
                /*
                if (who->is_bad() && me->is_good())
                {
                        message_sort(CYN "$N" CYN "大怒道：岂有此理！你居然已经误" 
                                     "入歧途，还敢来见我！\n" NOR, me, who); 
                        return 1; 
                } 
        
                if (who->is_good() && me->is_bad())
                {
                        message_sort(CYN "$N" CYN "大怒道：岂有此理！才几天不见，" 
                                     "你居然也学起假仁假义来了，快给我滚！\n" 
                                     NOR, me, who); 
                        return 1; 
                } 
                */
                
                who->delete("ultraquest");       
                // 以下原先设计四种任务类型，现缩减为一种任务。                 
                switch (random(4)) 
                {
                case 0:                                     
                        message_sort(CYN "$N" CYN "凝视了$n" CYN "许久，方才点头道：现在江"
                                     "湖正义不伸，邪道横行，腥风血雨不断，听说这一切都是有"
                                     "人在操纵，你给我去查出这个主谋是谁，提他的头颅来见我"
                                     "。\n" NOR, me, who);
                        break;
                case 1: 
                        message_sort(CYN "$N" CYN "对$n" CYN "道：最近江湖即将发生重大变故"
                                     "，即时将会腥风血雨，这背后肯定有人操纵，你给我去查出"
                                     "幕后主谋，提他的头颅来见我。\n" NOR, me, who);
                        break;
                case 2: 
                        message_sort(CYN "$N" CYN "对$n" CYN "道：听说有一股势力正在策划在"
                                     "江湖上掀起腥风血雨，妄图颠覆武林，你给我去查出幕后主"
                                     "谋是谁，提他的人头来见我。\n" NOR, me, who);
                        
                        break;
                case 3:
                        /*
                        ob = get_dir(NPC_DIR, 0);
                        file = NPC_DIR + ob[random(sizeof(ob))];  
                        who->set("ultraquest/type", "kill");   
                        who->set("ultraquest/file", file); 
                        obj = new(file);
                        name = obj->query("name");
                        id = obj->query("id");
                        who->set("ultraquest/name", name);
                        who->set("ultraquest/id", id);  
                        destruct(obj);
                        */
                        message_sort(CYN "$N" CYN "对$n" CYN "道：最近江湖掀起腥风血雨，我"
                                     "怀疑背后一定有主谋，你去给我查出背后主谋是谁，提他的"
                                     "头颅来见我。\n" NOR, me, who);
                        break;  
                }
                
                who->set("ultraquest/type", "kill");    
                // 执行提供线索的程序，必须预先设定提供者 
                who->set("ultraquest/npc1", me->query("id")); 
                who->set("ultraquest/name1", me->query("name"));
                
                generate_information(who); 
                return 1; 
        } 
        
        if (topic == "线索" || topic == "clue") 
        {
                if (! who->query("ultraquest/type")) 
                        return "你要问什么事？说话不明不白的。"; 
        
                if (who->query("ultraquest/finish") == 4) 
                        return "任务不是已经快完成了吗？还需要什么线索。"; 
        
                // 最多只能重新提供三次线索 
                if (who->query("ultraquest/begin") >= 3) 
                        return "事不过三，剩下的事情你自己想办法解决。"; 
        
                switch (who->query("ultraquest/begin")) 
                { 
                case 0: 
                        msg = "你这次是第一次，希望不会再有下回。"; 
                        break; 
        
                case 1: 
                        msg = "我已经帮过你一次了，这次是第二次。"; 
                        break; 
        
                default: 
                        msg = "这已经是最后一次了，望君好自为之。"; 
                        break; 
                } 
        
                message_sort(WHT "\n$N" WHT "皱了皱眉，对$n" WHT "道：我所交给你的"
                             "任务其实是一个历练的过程，很多地方必须经过磨练，凭借"
                             "自身的睿智与勇气才能办到。在这个过程中我最多可以帮你"
                             "三次。" + msg + "\n\n" NOR, me, who); 

                // 取消以前的任务属性                                        
                who->delete("ultraquest/quest");  // 分支任务
                who->delete("ultraquest/gift");   // 分支任务奖励                     
                who->delete("ultraquest/finish"); // 分支任务完成数
                who->delete("ultraquest/npc1");   // 第一线索 id
                who->delete("ultraquest/name1");  // 第一线索 name
                who->delete("ultraquest/npc2");   // 第二线索 id
                who->delete("ultraquest/name2");  // 第二线索 name
                who->delete("ultraquest/npc3");
                who->delete("ultraquest/name3");
                who->delete("ultraquest/npc4");
                who->delete("ultraquest/name4");
                who->delete("ultraquest/npc5");
                who->delete("ultraquest/name5");
                who->delete("ultraquest/place");  // 提供线索人物的地点
        
                // 记录重新询问的次数 
                who->add("ultraquest/begin", 1); 
        
                // 记录提供线索的人物名称 
                who->set("ultraquest/npc1", me->query("id")); 
                who->set("ultraquest/name1", me->query("name"));
        
                // 重新提供线索 
                generate_information(who); 
                return 1; 
        }
        
        if (topic == "取消" || topic == "cancel")  
        {
                   /*
                if (! who->query("ultraquest/type")) 
                        return "我没给你什么任务啊？";
                   */
                        
                who->add("weiwang", -50);
                if (who->query("weiwang") < 0)
                        who->set("weiwang", 0);
                 
                who->add("score", -100);
                if (who->query("score") < 0)
                        who->set("score", 0);
                        
                who->delete("ultraquest");
                who->delete("ultra_count");
                message_vision(CYN "$N" CYN "叹口气，看了看$n" CYN "道：连这点小事都干不了？算了吧。\n" NOR, 
                               me, who); 
                tell_object(who, HIR "你的江湖威望和江湖阅历受到了损失。\n" NOR);
                return 1;
        }
                 
} 
        
// 询问线索人物，需要在 /inherit/char/npc.c 增加对应程序 
public mixed ask_information(object me, object who, string topic) 
{ 
        string msg; 
        string type, ob; 
        string next_npc, npc_place; 

        if (who->query("ultraquest/npc5"))        
                next_npc = who->query("ultraquest/npc5"); 
        else
        if (who->query("ultraquest/npc4"))  
                next_npc = who->query("ultraquest/npc4"); 
        else
        if (who->query("ultraquest/npc3"))  
                next_npc = who->query("ultraquest/npc3"); 
        else
                next_npc = who->query("ultraquest/npc2"); 
                
        npc_place = who->query("ultraquest/place"); 
        
        if (stringp(type = who->query("ultraquest/quest/type")) &&
            stringp(ob = who->query("ultraquest/quest/name")) &&
            topic == ob) 
        { 
                // 提供任务人物对所提供出的任务信息的解答 
                if (me->query("id") == next_npc) 
                { 
                        switch (type) 
                        { 
                        case "kill" : 
                                msg = "那贼厮鸟，我…我恨不得把他碎尸万断！"; 
                                break; 
                        /*       
                        case "find" : 
                                msg = "说起来那东西也不难找，你自己多留意吧。"; 
                                break; 
                        */        
                        case "send" : 
                                msg = "其实那人我也不是很熟，你四处去问问吧。"; 
                                break; 
                        
                        case "give" : 
                                msg = "我很久没见到他了，也不清楚他现在怎样。"; 
                                break; 
                               
                        case "going" : 
                                msg = "我给你说得很清楚了啊，莫非你也不识路？"; 
                                break; 
                        } 
                        return msg; 
                } 
        } 
        
        if (topic == "线索" || topic == "clue") 
        { 
                // 询问目标为上一个提供信息的人   
                if (who->query("ultraquest/name5") &&
                    who->query("ultraquest/npc4") == me->query("id")) 
                        return "我不是让你去" + npc_place + "找" HIY + 
                               who->query("ultraquest/name5") + NOR 
                               + CYN "么？老来烦我做甚。" NOR;
               
                if (who->query("ultraquest/name4") &&
                    who->query("ultraquest/npc3") == me->query("id")) 
                        return "我不是让你去" + npc_place + "找" HIY + 
                               who->query("ultraquest/name4") + NOR 
                               + CYN "么？老来烦我做甚。" NOR; 
                                                                                          
                if (who->query("ultraquest/name3") &&
                    who->query("ultraquest/npc2") == me->query("id")) 
                        return "我不是让你去" + npc_place + "找" HIY + 
                               who->query("ultraquest/name3") + NOR 
                               + CYN "么？老来烦我做甚。" NOR;                                
                                                                                                     
                // 如果以前的任务无法执行，则中断 
                if (who->query("ultraquest/fail")) 
                        return "事情到了这一步，我也没法再帮你了。"; 
        
                // 如果已经给予了任务，则返回 
                if (who->query("ultraquest/quest/msg")) 
                        return "我不是让你" + 
                               who->query("ultraquest/quest/msg") + 
                               "吗？还罗嗦什么？"; 
        
                switch (random(4)) 
                { 
                case 0: 
                        msg = "你要找的线索，我确实知道一些，不过" 
                              "首先得帮我个忙。"; 
                        break; 
        
                case 1: 
                        msg = "我这里确实多多少少知道点眉目，不过" 
                              "消息可不能白给。"; 
                        break; 
        
                case 2: 
                        msg = "我正为一件事发愁，如果你能帮我这个" 
                              "忙，或许可以告诉你。"; 
                        break; 
        
                default: 
                        msg = "我自己的事情都忙不过来，哪里还顾得" 
                              "了你，不过…除非…"; 
                        break; 
                } 
                message_vision(CYN "$N" CYN "摇头道：" + msg + "\n" 
                               NOR, me, who); 
        
                // 如果条件符合要求，执行技术需求的任务 
                if (me->query("combat_exp") >= 100000 
                    && me->query("age") >= 20 
                    && ! me->query_temp("need_accede") 
                    && random(3) == 1) 
                { 
                        give_accede(me, who); 
                        return 1; 
                } 
        
                // 如果条件符合要求，执行保护或护送任务 
                if (who->query("combat_exp") > me->query("combat_exp") * 2 / 3 
                    && ! environment(who)->query("no_fight") 
                    && ! me->query_temp("start_guard") 
                    && random(3) == 1) 
                { 
                        /*
                        if (random(2) == 1) 
                        { 
                                // 执行护送的任务 
                                quest_going(me, who); 
                                return 1; 
                        } else 
                        { 
                        */
                                // 执行保护的任务 
                                quest_guard(me, who);
                                return 1; 
                        /*
                        } 
                        */
                } 
        
                // 给予系统预设任务 
                give_quest(who); 
                return 1; 
        } 
} 
        
// 技术性任务的发布 
public void give_accede(object me, object who) 
{ 
        string msg, type, quest_msg; 
        string skill; 
        string ma, mb, wa, ac; 
        int exp, lvl; 
        
        // 把人称及常用字符串作简写调用 
        ma = RANK_D->query_self(me); 
        mb = RANK_D->query_self_rude(me); 
        wa = RANK_D->query_respect(who); 
        ac = "(" HIY "accede" NOR + WHT ")"; 
        
        switch (random(6)) 
        { 
        case 0: 
                msg = random(2) ? "这位" + wa + "，" + ma + "近年来研究" 
                                  HIW "算术" NOR + WHT "易经，颇有心得。"
                                  "你我二人便来比试" + ac + "一盘，倘若" 
                                  "你能胜过我。那么" + ma + "所知道的线" 
                                  "索则无条件奉告": 
        
                                  wa + "来得正好，" + mb + "正愁没事做。" 
                                  "来来来，你陪我比试" + ac + "一盘" HIW 
                                  "算术" NOR + WHT "，倘若你能胜过我手中" 
                                  "的竹签与算盘，我把我知道的一切都告诉你"; 
        
                quest_msg = "跟我比试算术"; 
                type = "mathematics"; 
                break; 
        
        case 1: 
                msg = random(2) ? "这位" + wa + "，" + ma + "近年来研究" 
                                  HIW "诗词" NOR + WHT "歌赋，感触颇深。" 
                                  "你我二人便来吟诗" + ac + "作对，倘若" 
                                  "你能对赢" + ma + "。那么我所知道的线" 
                                  "索则无条件奉告": 
        
                                  "我看" + wa + "也无甚要事，既然你我二" 
                                  "人都穷极无聊，不如来吟诗" + ac + "作" 
                                  "对，共同探讨探讨" HIW "文学" NOR + WHT 
                                  "知识。倘若你真能胜我，" + ma + "即将" 
                                  "线索告之"; 
        
                quest_msg = "跟我吟诗对对"; 
                type = "literate"; 
                break; 
        
        case 2: 
                msg = random(2) ? "这位" + wa + "，" + ma + "近年来研究" 
                                  HIW "弈棋" NOR + WHT "之道，颇有心得。" 
                                  "你我二人便来对弈" + ac + "一局，倘若" 
                                  "你能胜过我。那么我所知道的线索则无条" 
                                  "件奉告": 
        
                                  wa + "你过来，我看你也没甚要紧事做。干" 
                                  "脆来陪我杀" + ac + "一局" HIW "棋" NOR + 
                                  WHT "，省得无聊。倘若你真能胜我几子，我" 
                                  "便把知道的全都告诉你也无妨"; 
        
                quest_msg = "陪我下一局棋"; 
                type = "chess"; 
                break; 
        
        case 3: 
                msg = random(2) ? "这位" + wa + "来得正好，" + ma + "正为" 
                                  "一事发愁。今趟是我朋友的大寿，" + ma + 
                                  "打算送他一幅对联以表祝贺，可怎奈" HIW 
                                  "书法" NOR + WHT "功夫欠佳，倘若你能帮" + 
                                  ac + "我达成这个心愿…" + ma + "感激不" 
                                  "尽，线索也当无条件奉告": 
        
                                  wa + "来得正是时候，" + ma + "正为一事" 
                                  "发愁。唉…说起来…前日我一位挚友为奸贼" 
                                  "所害，不幸惨死。今趟" + ma + "打算寻求" 
                                  "一位" HIW "书法" NOR + WHT "名家为其书" 
                                  "写悼词，却不知" + wa + "能否帮" + ac + 
                                  ma + "达成这个心愿"; 
        
                quest_msg = "帮我写一幅字"; 
                type = "calligraphy"; 
                break; 
        
        case 4: 
                msg = random(2) ? "这位" + wa + "来得正好，" + ma + "正为" 
                                  "一事发愁。前日我朋友喜得贵子，" + ma + 
                                  "打算送他一幅喜图以表祝贺，可怎奈" HIW 
                                  "丹青" NOR + WHT "造诣太差，倘若你能帮" + 
                                  ac + "我达成这个心愿…" + ma + "感激不" 
                                  "尽，线索也当无条件奉告": 
        
                                  "这个…" + ma + "有个不情之请，还请" + wa + 
                                  "帮忙" + ac + "达成。" + ma + "有个多年" 
                                  "不见的知己，乃是位" HIW "丹青" NOR + WHT 
                                  "名手，今趟去拜访…却不知该如何备礼。倘" 
                                  "若" + wa + "有此之长，能够投其所好，当" 
                                  "真感激不尽"; 
        
                quest_msg = "帮我画一幅画"; 
                type = "drawing"; 
                break; 
        
        default: 
                msg = random(2) ? "嗯…" + wa + "你过来，昨日" + ma + "跟" 
                                  "人动手，不小心伤了筋骨，今日本想医治…" 
                                  "可怎奈附近找不到一位" HIW "医术" NOR + 
                                  WHT "好点的大夫，若你能帮忙" + ac + "治" 
                                  "好此伤，" + ma + "感激不尽，线索也当无" 
                                  "条件奉告": 
        
                                  "不瞒你讲，" + wa + "，前几日" + ma + 
                                  "与人过招，不慎受了点内伤，至尽尚未痊愈" 
                                  "。唉…这病痛之苦委实难熬，倘若阁下通晓" 
                                  HIW "医理" NOR + WHT "，能够减轻" + ac + 
                                  ma + "病痛的话，" + ma + "当把知道的一切" 
                                  "全部告之"; 
        
                quest_msg = "帮我医治病痛"; 
                type = "medical"; 
                break; 
        } 
        
        // 提供出任务的描述信息 
        msg = WHT "\n" + me->name() + WHT "沉思片刻，对你" 
              "说道：" + msg + "。\n\n" NOR; 
        
        message("vision", me->name() + "在" + who->name() + 
                "耳边小声说了些什么。\n", 
                environment(who), ({ who })); 
        
        tell_object(who, sort_msg(msg)); 
        
        // 技能要求的种类 
        skill = type; 
        
        // 为什么乘十？是为了目标最大经验等级的计算 
        exp = me->query("combat_exp") * 10; 
        
        // 如果该人物有该项技能，则直接调用 
        // 如果没有，则取最大经验支持等级作削减 
        if (me->query_skill(skill)) 
                lvl = me->query_skill(skill, 1); 
        else 
                lvl = pow(exp, 1.0 / 3) - 50; 
        
        // 等级太低则提高 
        if (lvl < 80) lvl = 80; 
        
        // 等级太高则削弱 
        if (lvl > 300) lvl = 300; 
        
        /* 
        tell_object(who, HIC "【测试信息】" NOR + WHT + me->name() + 
                         HIW " 经验：" HIC + exp / 10 + 
                         HIW " 类别：" HIC + to_chinese(skill) + 
                         HIW " 等级：" HIC + lvl + "\n\n" NOR); 
        */ 
          
        // 注册任务属性 
        who->set("ultraquest/quest/type", type); 
        who->set("ultraquest/quest/name", me->query("name")); 
        who->set("ultraquest/quest/id", me->query("id"));
        who->set("ultraquest/quest/msg", quest_msg); 
        
        // 人物注册接受玩家 
        me->set_temp("need_accede/user", who->query("id")); 
        me->set_temp("need_accede/lvl", lvl);  
        
        return; 
} 
         
// 系统预设任务的发布 
public void give_quest(object me) 
{ 
        string *lines, file; 
        string quest, next_npc, msg, npc; 
        string type, next_name, quest_msg, name, place; 
        object item, ob; 

        if (me->query("ultraquest/npc5"))  
        {
                npc = me->query("ultraquest/npc5");
                name = me->query("ultraquest/name5"); 
        } else        
        if (me->query("ultraquest/npc4"))  
        {
                npc = me->query("ultraquest/npc4");
                name = me->query("ultraquest/name4");             
        } else
        if (me->query("ultraquest/npc3"))  
        {
                npc = me->query("ultraquest/npc3"); 
                name = me->query("ultraquest/name3");
        } else
        if (me->query("ultraquest/npc2"))
        { 
                npc = me->query("ultraquest/npc2");
                name = me->query("ultraquest/name2");   
        } else
        {  
                npc = me->query("ultraquest/npc1"); 
                name = me->query("ultraquest/name1");
        }
        
        // 没有提供线索的人 
        if (! npc) return; 
                                
        switch (random(3)) 
        { 
        case 0 : 
                type = "kill";
                ob = new(CLASS_D("generate") + "/killer.c");
                NPC_D->place_npc(ob, ({ "大理一带", "终南山", "关外", "长安城", "杭州城" }));
                NPC_D->set_from_me(ob, me, 120); 
                ob->add_temp("apply/attack", ob->query_skill("force") * 1 / 2);
                ob->add_temp("apply/dodge", ob->query_skill("force") * 1 / 2);
                ob->add_temp("apply/parry", ob->query_skill("force") * 1 / 2);
                ob->add_temp("apply/damage", 500);
                ob->add_temp("apply/unarmed_damage", 500);
                ob->add_temp("apply/armor", 500);
                ob->set_temp("quester", me->query("id"));                              
                                         
                NPC_D->random_move(ob);
                NPC_D->random_move(ob);

                next_npc = ob->query("id");
                next_name = ob->query("name"); 
                place = ob->query("place");   
                                                
                msg = random(2) ? "你帮我杀了" HIR + next_name + "(" + next_npc + ")" + NOR + 
                                  WHT "那厮，再提他的头来见我。\n听说他最近在" + place + "，你快去吧！": 
        
                                  "你去把" HIR + next_name + "(" + next_npc + ")" + NOR + WHT 
                                  "给我做了，提他项上的头来见我。\n听说他最近在" + place + "，你快去吧！"; 
        
                quest_msg = "帮我杀了" + next_name + "(" + next_npc + ")"; 
                break; 
        /*
        case 1 : 
                type = "find";
                msg = random(2) ? "我现在正急需" + next_name + "(" + next_npc + ")" + NOR + 
                                  WHT "，只好劳烦劳烦阁下了": 
        
                                  "倘若你能帮我找到" + next_name + "(" + next_npc + ")" + NOR 
                                  + WHT "，或许我可以提供些线索"; 
        
                quest_msg = "帮我找到" + next_name + "(" + next_npc + ")"; 
                break; 
        */
        case 1 : 
                type = "send";
                ob = new(CLASS_D("generate") + "/receiver1.c"); 
                NPC_D->place_npc(ob, ({ "大理一带", "终南山", "关外", "长安城", "杭州城" }));
                NPC_D->set_from_me(ob, me, 100); 
        
                NPC_D->random_move(ob); 
                
                next_npc = ob->query("id");
                next_name = ob->query("name");             
                place = ob->query("place"); 
                                
                msg = random(2) ? "你帮我把这封信送到" HIY + next_name + "(" + next_npc + ")" 
                                  + NOR + WHT "手中，然后拿" HIW 
                                  "回执" NOR + WHT "来见我。\n听说他最近在" + place + "，你快去吧！": 
        
                                  "我有封信要立刻送到" HIY + next_name + "(" + next_npc + ")" 
                                  + NOR + WHT "处，这件事你帮我" 
                                  "办办吧。\n听说他最近在" + place + "，你快去吧！"; 
        
                quest_msg = "帮我把信送到" + next_name + "(" + next_npc + ")" + "处"; 
                item = new("/clone/misc/letter"); 
                break; 
        
        case 2 : 
                type = "give";

                ob = new(CLASS_D("generate") + "/receiver1.c"); 
                NPC_D->place_npc(ob, ({ "大理一带", "终南山", "关外", "长安城", "杭州城" }));
                NPC_D->set_from_me(ob, me, 100); 
        
                NPC_D->random_move(ob); 

                next_npc = ob->query("id");
                next_name = ob->query("name");             
                place = ob->query("place"); 
                                
                msg = random(2) ? "你帮我把这个包裹送到" HIY + 
                                  next_name + "(" + next_npc + ")" + NOR + WHT "那里，看看他" 
                                  "有什么反应。\n听说他最近在" + place + "，你快去吧！": 
        
                                  "劳烦将此包裹转交给" HIY + next_name + "(" + next_npc + ")"
                                  + NOR + WHT "，中途切莫私自打" 
                                  "开。\n听说他最近在" + place + "，你快去吧！"; 
        
                quest_msg = "帮我把包裹转交给" + next_name + "(" + next_npc + ")"; 
                item = new("/clone/misc/baoguo"); 
                break; 
        } 
        
        // 提供出线索信息 
        message("vision", name + "悄悄在" + me->name() + "耳" 
                          "边说了些什么。\n", environment(me), 
                          ({ me })); 
        
        tell_object(me, WHT + name + WHT "悄悄对你说道：" + 
                        msg + "。\n" NOR); 
        
        // 注册任务属性 
        me->set("ultraquest/quest/type", type); 
        me->set("ultraquest/quest/name", next_name); 
        me->set("ultraquest/quest/id", next_npc);
        me->set("ultraquest/quest/msg", quest_msg); 
        
        // 如果存在任务道具，则注册物件属性 
        if (objectp(item)) 
        { 
                item->set("send/from", name); 
                item->set("send/send", next_name); 
                item->set("send/user", me->name()); 
                item->set("long", WHT "这是一" + item->query("unit") + 
                                  name + "准备转交给" + next_name + "的" + 
                                  item->name() + "。\n" NOR); 
        
                // 提供出物件转移的信息 
                message_vision(name + "拿出一" + item->query("unit") + 
                               item->name() + NOR "(" + item->query("id") + 
                               ")给$N" NOR "。\n" NOR, me); 
                item->move(me, 1); 
        } 
        return; 
} 
        
// 保护任务的发布 
public void quest_guard(object me, object who) 
{ 
        string msg; 
        
        message("vision", me->name() + "在" + who->name() + 
                          "耳边小声说了些什么。\n", 
                          environment(who), ({ who })); 
        
        switch (random(3)) 
        { 
        case 0: 
                msg = "我的仇家今日要杀上门来"; 
                break; 
        
        case 1: 
                msg = "昨日有个家伙放言要做了我"; 
                break; 
        
        default: 
                msg = "我听说有人准备对我不利"; 
                break; 
        } 
        
        tell_object(who, WHT + me->name() + WHT "悄悄对你" 
                         "说道：" + msg + "，你" HIW "留在" 
                         "这里" NOR + WHT "保护我一下。\n" NOR); 
        
        // 为了防止任务冲突，目标设置被保护属性 
        me->set_temp("start_guard", 1); 
          
        // 注册保护任务属性 
        who->set("ultraquest/quest/type", "guard"); 
        who->set("ultraquest/quest/name", me->query("name")); 
        who->set("ultraquest/quest/id", me->query("id"));
        who->set("ultraquest/quest/msg", "留在这里保护我"); 
        who->start_call_out((: call_other, __FILE__, "start_guard", 
                            me, who :), 15); 
        return; 
} 
        
// 护送任务的发布 
public void quest_going(object me, object who) 
{ 
        string *lines, file; 
        string where, area, place; 
        
        // 调用系统预设的地点名称 
        file = read_file(QUEST + "place"); 
        lines = explode(file, "\n"); 
        where = lines[random(sizeof(lines))]; 
        
        if (sscanf(where, "%s/%s", area, place) != 2) 
                return; 
        
        // 如果系统调用的地名与起始地重复，则直接执行 
        // 系统预设的任务程序 

        if (place == environment(me)->query("short")) 
        { 
                generate_information(who); 
                return; 
        } 

        message("vision", me->name() + "在" + who->name() + 
                          "耳边小声说了些什么。\n", 
                          environment(who), ({ who })); 
        
        tell_object(who, WHT + me->name() + WHT "悄悄对你" 
                         "说道：我现在有点事要到" + HIW + 
                         area + NOR + WHT "的" HIW + place + 
                         NOR + WHT "处理，劳驾你送我过去。" 
                         "\n" NOR); 
        
        // 目标开始跟随行动 
        me->set_leader(who); 
        message_vision("$N决定开始跟随$n一起行动。\n", me, who); 
        
        // 为了防止任务冲突，目标设置被保护属性 
        me->set_temp("start_guard", 1); 
        
        // 注册护送任务属性 
        who->set("ultraquest/quest/type", "going"); 
        who->set("ultraquest/quest/obj", place); 
        who->set("ultraquest/quest/name", me->query("name"));
        who->set("ultraquest/quest/msg", "送我到" + area + "的" + place); 
        
        // 开始定时扫描所到达的地点 
        who->start_call_out((: call_other, __FILE__, "check_going", 
                            me, who :), 1); 
        return; 
} 
         
// 执行保护任务的程序 
void start_guard(object me, object who) 
{ 
        object killer; 
        
        if (! objectp(me) || ! objectp(who) 
            || environment(me) != environment(who)) 
                return; 
        
        if (random(5) == 1) 
        { 
                message_vision(CYN "\n$N" CYN "挠了挠头，颇为" 
                               "尴尬的说道：咦？难道传闻有错？" 
                               "\n" NOR, me); 
        
                if (random(3) == 1) 
                { 
                        message_vision(CYN "$N" CYN "干笑了两" 
                                       "声，对$n" CYN "道：看" 
                                       "样子没人来了，你也算辛" 
                                       "苦了一场，我就告诉你吧" 
                                       "。\n" NOR, me, who); 
        
                        // 目标取消被保护的属性 
                        me->delete_temp("start_guard"); 
        
                        // 任务完成，注册奖励属性 
                        // who->set("ultraquest/name1", me->name()); 
                        who->set("ultraquest/gift/point", 120); 
                        who->set("ultraquest/gift/msg", "你通过保护" + 
                                 who->query("ultraquest/quest/name") + "的过程"); 
        
                        // 系统给予该次任务的奖励 
                        give_gift(who); 
                } else 
                { 
                        message_vision(CYN "$N" CYN "叹了口气" 
                                       "，对$n" CYN "道：看样" 
                                       "子没人来了，我还是重新" 
                                       "给你个事情做做吧。\n" 
                                       NOR, me, who); 
        
                        // 目标取消被保护的属性 
                        me->delete_temp("start_guard"); 
        
                        // 任务中断，系统重新分配的任务 
                        give_quest(who); 
                } 
                return; 
        } 
        message_vision(CYN "\n突然间$N" CYN "对$n" CYN "颤声说道" 
                       "：来了，杀我的人来了。\n\n" NOR, me, who); 
        
        killer = new(CLASS_D("generate") + "/killer"); 
        NPC_D->set_from_me(killer, who, 120); 
        killer->move(environment(me)); 
        
        message_vision(HIW "$n" HIW "走了过来，对$N" HIW "狞笑" 
                       "道：原来你躲在这里，受死吧！\n" NOR, me, 
                       killer); 
        
        killer->set_leader(me); 
        killer->kill_ob(me); 
        
        // 检查保护状态 
        who->start_call_out((: call_other, __FILE__, "check_guard", 
                             me, who, killer :), 3); 
} 
        
// 检查被保护任务的状态 
void check_guard(object me, object who, object killer) 
{ 
        if (! objectp(me) && ! objectp(killer)) 
                return; 
        
        // 正在战斗？则延迟检查 
        if (objectp(killer) && objectp(me) 
            && (me->is_fighting(killer) 
            || who->is_fighting(killer))) 
        { 
                who->start_call_out((: call_other, __FILE__, "check_guard", 
                                    me, who, killer :), 1); 
                return; 
        } 
        
        // 被保护的人消失，任务中断 
        if (! objectp(me) 
            && objectp(killer)) 
        { 
                tell_object(who, HIG "你眼见" + who->query("ultraquest/quest/name") + 
                                 HIG "被杀，心中连连叫糟。\n" NOR); 
        
                // 记录下失败记录 
                who->set("ultraquest/fail", 1); 
                return; 
        } 
        
        // 杀手消失且被保护人存在，任务结束 
        if (! objectp(killer) 
            && objectp(me)) 
        { 
                // 如果被保护人昏迷，则先转醒 
                if (! living(me)) 
                        me->revive(); 
        
                message_vision(CYN "$N" CYN "擦了擦汗，对$n" CYN "道：天幸" 
                               "今天有你帮忙，感激不尽。\n$N" CYN "接着道：" 
                               "救命之恩难以回报，我一定把我知道的都告诉你" 
                               "。\n" NOR, me, who); 
        
                // 目标取消被保护的属性 
                me->delete_temp("start_guard"); 
        
                // 任务完成，注册奖励属性 
                // who->set("ultraquest/name1", me->name()); 
                who->set("ultraquest/gift/point", 250); 
                who->set("ultraquest/gift/msg", "你通过保护" + 
                who->query("ultraquest/quest/name") + "的过程"); 
        
                // 系统给予该次任务的奖励 
                give_gift(who); 
                return; 
        } 
        message_vision(CYN "$N" CYN "看了看四周，大叫道：巫" 
                       "师！BUG，BUG！\n" NOR, who); 
        return; 
} 
          
// 检查护送任务的状态 
void check_going(object me, object who) 
{ 
        string msg, place; 
        
        // 若任务已经失败，则取消掉属性并且中断任务 
        if (who->query("ultraquest/fail")) 
        { 
                who->delete_temp("dest_npc"); 
                return; 
        } 
        
        // 特殊情况下护送的人物跟丢了，执行以下程序 
        if (! objectp(me) || ! objectp(who)) return;

        // 调用出护送的目的地 
        place = who->query("ultraquest/quest/obj"); 
                
        if (environment(me) != environment(who)) 
        { 
                switch (who->query_temp("dest_npc")) 
                { 
                case 0: 
                        msg = "你回头张望间，却发现那" + me->name() 
                              + "竟不知何时不见了踪影。\n你心中暗道" 
                              "：可千万别让线索断掉了，得赶紧将他找" 
                              "回来。"; 
                        break; 
        
                case 1: 
                        msg = "你心中颇为焦急：咦？" + me->name()
                              + "那家伙到底上哪去了。"; 
                        break; 
        
                case 2: 
                        msg = "你心底直骂：什么时候了，那该死的" + 
                              me->name() + "怎么还不滚出来。"; 
                        break; 
        
                default: 
                        msg = "你心中连连叹息：唉…到底还是把人给" 
                              "跟丢了。这线索…唉…"; 
        
                        // 记录下失败记录 
                        who->set("ultraquest/fail", 1); 
                        break; 
                } 
        
                // 设置人物丢失的时间段，超过三则任务中断 
                who->add_temp("dest_npc", 1); 
        
                // 等一个时间段再重新做扫描 
                who->start_call_out((: call_other, __FILE__, "check_going", 
                                    me, who :), 5); 
        
                tell_object(who, HIG "\n" + msg + "\n\n" NOR); 
                return; 
        } 
        
        // 清除等候护送人物的时间段 
        if (who->query_temp("dest_npc")) 
                who->delete_temp("dest_npc");        
        
                                       
        // 查看当前位置，如为目的地，则任务完成 
        if (environment(me)->query("short") == (string)who->query("ultraquest/quest/obj")) 
        { 
                // 如果被保护人昏迷，则先转醒 
                if (! living(me)) 
                        me->revive(); 
        
                message_vision(CYN "$N" CYN "擦了擦汗，对$n" CYN "道：谢谢" 
                               "你将我送到" + place + "，路上真是麻烦你了。" 
                               "\n$N" CYN "接着道：看在你这番辛苦的份上，我" 
                               "就告诉你一些线索吧。\n" NOR, me, who); 
        
                // 目标取消被保护的属性 
                me->delete_temp("start_guard"); 
        
                // 目标不再跟随 
                me->set_leader(0); 
        
                // 任务完成，注册奖励属性 
                // who->set("ultraquest/name1", me->name()); 
                who->set("ultraquest/gift/point", 220); 
                who->set("ultraquest/gift/msg", "你通过护送" + 
                                                 who->query("ultraquest/quest/name") + "抵达" + 
                                                 place + "的过程"); 
        
                // 系统给予该次任务的奖励 
                give_gift(who); 
                return; 
        } else 
        { 
                string last_place; 
        
                // 随机事件使经验较低的护送目标停留 
                if (stringp(last_place = me->query_temp("last_place")) 
                    && last_place != environment(me)->query("short") 
                    && ! me->is_busy() 
                    && me->query("combat_exp") < 2000000 
                    && random(50) == 1) 
                { 
                        switch (random(5)) 
                        { 
                        case 0: 
                                msg = "突然间$N" HIW "跑得急了，一个筋斗" 
                                      "栽到路边，咿咿呀呀叫个不停。"; 
                                break; 
        
                        case 1: 
                                msg = "只听$N" HIW "喘息道：" + who->name() 
                                      + HIW "，走慢点…走慢…累死我了。"; 
                                break; 
        
                        case 2: 
                                msg = "$N" HIW "「咦」了一声，似乎发现了" 
                                      "什么新鲜事物，脚步顿时缓了下来。"; 
                                break; 
        
                        case 3: 
                                msg = "$N" HIW "忽然脸色惨白，道：" + 
                                      who->name() + HIW "，劳烦停…停一" 
                                      "会，我走岔气了。"; 
                                break; 
        
                        default: 
                                msg = "却见$N" HIW "不留神间，踢到了路边" 
                                      "凸起的一块大石，直痛得躬下腰去。"; 
                                break; 
                        } 
                        message_vision(HIW "\n" + msg + "\n\n" NOR, me, who); 
                        me->start_busy(2 + random(2)); 
                } 
        
                // 设定上一次所到达的地点，如果与当前地点不同，则 
                // 执行上面的随机停留的程序 
                me->set_temp("last_place", environment(me)->query("short")); 
        
                // 未到达目的地，定时重新做扫描 
                who->start_call_out((: call_other, __FILE__, "check_going", 
                                    me, who :), 1); 
                return; 
        } 
        return; 
} 
          
// 针对任务人物提出的技术性任务给予答复 
int need_accede(object me, object who) 
{ 
        string skill, ma, wa; 
        string suc_msg, fai_msg, msg; 
        int lvl; 
        
        // 准备比较的技能属性 
        skill = who->query("ultraquest/quest/type"); 
        lvl = me->query_temp("need_accede/lvl", lvl); 
           
        // 把人称做简单化 
        ma = RANK_D->query_self(me); 
        wa = RANK_D->query_respect(who); 
        
        switch (skill) 
        { 
        case "mathematics" : 
                suc_msg = me->name() + "点了点头，随即蹲下身来，在地面画" 
                          "了些横直符号与圆圈接着又写下「太、天元、地元、" 
                          "人元、物元」等字。你在一旁默默细观，掐指间便已" 
                          "将几道算题的答案算出，随口告之" + me->name() + 
                          "。" + me->name() + "听后一惊，叹道：我原以为…" 
                          "唉…罢了，罢了。"; 
        
                fai_msg = me->name() + "点了点头，随即蹲下身来，在地面画" 
                          "了些横直符号与圆圈接着又写下「太、天元、地元、" 
                          "人元、物元」等字。你在一旁看得不知所云，全然不" 
                          "知该如何下手。" + me->name() + "等了半天，见你" 
                          "毫无动静，不由得满意的一笑。"; 
                break; 
        
        case "literate" : 
                suc_msg = me->name() + "点了点头，清了清嗓子道：我出一个" 
                          "上联，你给我对出下联。这上联是「炎黄英雄史」。" 
                          "你微微一笑，随口对道：「网络傻瓜墓」。" + 
                          me->name() + "听后一震，赞道：绝妙，真乃绝妙。"; 
        
                fai_msg = me->name() + "点了点头，清了清嗓子道：我出一个" 
                          "上联，你给我对出下联。我这上联是「炎黄英雄史」" 
                          "。你听后一愣，苦想了半天才道：「暗…暗黑破坏神" 
                          "」。" + me->name() + "听后大怒，骂道：我呸，这" 
                          "算什么对子。你就知道黄对黑么？"; 
                break; 
        
        case "chess" : 
                suc_msg = "你当即走上前去，与" + me->name() + "摆开棋局" 
                          "对弈起来。不过多时，却见" + me->name() + "已" 
                          "是大汗淋漓。紧接着你又落下一子，" + me->name() + 
                          "顿时脸色铁青，颓然道：不下了，不下了。我认输" 
                          "便是。"; 
        
                fai_msg = "你当即走上前去，与" + me->name() + "摆开棋局" 
                          "对弈起来。不过多时，你已被" + me->name() + 
                          "逼得节节败退，险象横生。" + me->name() + "见" 
                          "你脸色铁青，大笑道：怎么？就你这水平，还不认" 
                          "输么？"; 
                break; 
        
        case "calligraphy" : 
                suc_msg = "你微微一笑，当即走上前去，接过毛笔便开始书写" 
                          "起来。霎时只见你随意挥洒手中笔墨，腾蛟起凤，" 
                          "洋洋洒洒；字迹宛若行云流水，当真将书法的意境" 
                          "发挥得淋漓尽致。" + me->name() + "在一旁看得" 
                          "目瞪口呆，连声称赞。"; 
        
                fai_msg = "你当即走上前去，接过毛笔饱沾墨水，便开始书写" 
                          "起来。正当你写到意气风发之处，抬头间却发现" + 
                           me->name() + "颜色甚为难看，不由得微微一愣。" + 
                           me->name() + "摇了摇头，失望道：倘若我将这样的" 
                           "字拿出手，岂非惹人耻笑？原本还…唉…"; 
                break; 
        
        case "drawing" : 
                suc_msg = "你微微一笑，当即接过画笔，随意挥洒笔墨。但见" 
                          "你墨意淋漓，笔力雄健，虽只寥寥数笔，气势可着" 
                          "实不凡。" + me->name() + "在一旁越看越是心喜" 
                          "，连声称赞。"; 
        
                fai_msg = "你当即走上前去，接过画笔作起画来。正当你画得" 
                          "意气风发之时，抬头间却发现" + me->name() + "颜" 
                          "色甚为难看，不由得微微一愣。" + me->name() + 
                          "摇了摇头，失望道：算了，算了。就你这水平，还" 
                          "没我自己动手画妥当。"; 
                break; 
        
        default: 
                suc_msg = "你点了点头，伸出两指搭在" + me->name() + "的" 
                          "脉上，不出片刻便已发现其病根所在。当下也不耽" 
                          "搁，急急配出了药方。" + me->name() + "见后大" 
                          "喜道：天幸今日碰上了" + wa + "，此番恩情真是" 
                          "难以回报。"; 
        
                fai_msg = "你愣了愣，走上前去给" + me->name() + "搭脉，" 
                          "探了半天，却发现" + me->name() + "脉象异常絮" 
                          "乱，远超自己所识。" + me->name() + "见你愁眉" 
                          "不展，不由得长叹道：唉…我这伤本非一般庸医能" 
                          "治，也不难为你了。"; 
                break; 
        } 
        
        message("vision", "只见" + who->name() + "走上前去，在" + 
                          me->name() + "耳边嘀咕了几句。\n", 
                          environment(who), ({ who })); 
        
        if (who->query_skill(skill, 1) >= lvl) 
        { 
                // 提交成功的描述信息 
                tell_object(who, sort_msg(WHT + suc_msg + "\n\n" NOR)); 
        
                // 任务完成，注册奖励属性 
                // who->set("ultraquest/name1", me->name()); 
                who->set("ultraquest/gift/point", 350); 
                who->set("ultraquest/gift/msg", "你通过此番帮助" + 
                                                 me->name() + "的经历"); 
        
                if (! who->is_busy()) 
                        who->start_busy(3); 
        
                // 随机给予奖励物品 
                if (random(5) == 1) 
                { 
                        message_vision(CYN "$N" CYN "道：" + wa + "的为人" 
                                       "实令" + ma + "折服不已。一点薄礼，" 
                                       "不成敬意。\n" NOR, me); 
        
                        who->set("ultraquest/gift/item", 1); 
                } 
        
                // 系统给予该次任务的奖励 
                give_gift(who); 
        } else 
        { 
                // 提交失败的描述信息 
                tell_object(who, sort_msg(WHT + fai_msg + "\n\n" NOR)); 
        
                message_vision(CYN "$N" CYN "摇了摇头，对$n" 
                               CYN "道：也罢，我还是重新交代" 
                               "个事情让你做吧。\n" NOR, me, who); 
        
                // 任务中断，系统重新分配的任务 
                give_quest(who); 
        } 
        // 注销掉人物的任务对象属性 
        me->delete_temp("need_accede"); 
        
        return 1; 
}
          
// 接受任务需求的物品 
int accept_object(object me, object who, object ob) 
{ 
        string msg; 
        string type, npc; 
        int ultra_count;
        int exp;                // 获得的经验
        int pot;                // 获得的潜能
        int mar;                // 获得的实战体会
        int weiwang;            // 获得的威望
        int score;              // 获得的江湖阅历
        int lvl;                // 人物的等级
        mixed special = 0;      // 是否有特殊奖励
        object ob1, rob;
        
        // 如果没有领取任务 
        if (! mapp(who->query("ultraquest"))) 
                return 0; 
     
        if (me->query("name") == QUESTER)
        {                        
                switch (who->query("ultraquest/type"))
                {
                case "find":
                        if (ob->query("id") != who->query("ultraquest/id"))
                                return 0;
                                
                        message_vision(CYN "$N" CYN "看了看" + ob->name() 
                                       + NOR + CYN "，微微的点了点头。\n" NOR, me); 
                        // destruct(ob); 
                        break;
                        
                case "give":
                case "rescue":
                        if (! ob->is_receipt())
                                return 0;
                        
                        if (ob->query("reply_to") != me->query("name")
                        ||  ob->query("reply_by") != who->query("ultraquest/id")
                        ||  ob->query("receive_from_name") != who->name(1))
                        {
                                message_vision("$N皱眉道：“给我这玩艺干什么？”\n", me);
                                return 0;
                        }
                        
                        message_vision(CYN "$N" CYN "接过" + ob->name() + NOR + CYN
                                       "，仔细看了看，满意的说：很好。\n" NOR, me); 
                        // destruct(ob);
                        break;
                        
                case "kill":
                        if(! ob->is_corpse() && ! ob->is_head())
                                return 0;
                                
                        if (! stringp(ob->query("owner_id"))) 
                        { 
                                message_vision(CYN "$N" CYN "捂着鼻子看了看" + ob->name() 
                                               + NOR + CYN "，道：什么臭烘烘的？快给我" 
                                               "拿走。\n" NOR, me); 
                                return 0; 
                        }

                        if (ob->query("killed_by") != who->query("id"))
                                return 0;
                                      
                        if (ob->query("victim_name") != who->query("ultraquest/name"))
                                return 0;
                                
                        // message_vision(CYN "$N" CYN "看了看" + ob->name() 
                        //                + NOR + CYN "，微微的点了点头。\n" NOR, me); 
                        // destruct(ob); 
                        break;
                }
                
                msg = CYN "$N" CYN "接过" + ob->name() + NOR + CYN"，仔细看了" 
                      "看，满意的说：很好。\n" NOR; 
                      
                // 这里要定义完成任务奖励
                who->add("ultra_count", 1);                 
                
                ultra_count = who->query("ultra_count");

                lvl = NPC_D->check_level(who); 
                exp = 18000 + random(5000) + lvl * 10; 
                pot = 18000 + random(3000) + lvl * 10; 
                mar = 1000 + random(1000); 
                weiwang = 200 + random(300) + lvl * 10; 
                score = 200 + random(300) + lvl * 10; 
                
                // 根据任务完成的次数修正奖励
                if (ultra_count >= 100)
                {
                        // 连续完成了超过100次的任务
                        exp += 800 + random(ultra_count + 1);
                        pot += 450 + random(ultra_count + 1);
                        mar += 500 + random(200);
                        weiwang += 80 + random(200);
                        score += 40 + random(150);
                } else 
                if (ultra_count >= 50) 
                { 
                        // 连续完成了超过50次的任务 
                        exp += 700 + random(ultra_count + 1); 
                        pot += 400 + random(ultra_count + 1); 
                        mar += 200 + random(100); 
                        weiwang += 50 + random(150); 
                        score += 30 + random(100); 
                } else 
                if (ultra_count >= 25)
                {
                        // 连续完成了超过25次的任务
                        exp += 500 + random(ultra_count + 1);
                        pot += 300 + random(ultra_count + 1);
                        mar += 100 + random(50); 
                        weiwang += 30 + random(100);
                        score += 20 + random(100);
                } else
                if (ultra_count >= 10)
                {
                        // 连续完成了超过10次的任务
                        exp += 450 + random(ultra_count + 1);
                        pot += 250 + random(ultra_count + 1);
                        weiwang += 10 + random(50);
                        score += 10 + random(50);
                }
                
                if (ultra_count == 10)
                {
                        msg += CYN "$N" CYN "哈哈大笑两声道：“真有你的，连着上十"
                               "次任务都完成的漂漂亮亮，很好，很好。”\n" NOR;
			special = 1;
                } else
                if (ultra_count == 25)
                {
                        msg += CYN "$N" CYN "大喜道：“不错不错，真实不错，二十五"
                               "次任务干得很好。”\n NOR";
                        special = "/clone/fam/max/naobaijin";
                } else
                if (ultra_count == 50)
                {
                        msg += CYN "$N" CYN "叹道：“真是长江后浪推前浪，想不到你接连五十"
                               "次任务都无一疏漏，不易，不易！”\n" NOR;
                        special = "/clone/fam/max/longjia";
                } else
                if (ultra_count >= 100)
                {
                        msg += CYN "$N" CYN "深深叹了口气，道：“想不到你连着上百次"
                               "任务无一失手，看来我们泥潭真是后继有人啊！”\n" NOR;
                        special = "/clone/fam/max/zhenyu";
                        ultra_count = 0;
                } else
                if ((ultra_count % 10) == 0)
                {
                        msg += CYN "$N" CYN "喜道：“真是不赖，居然连着" +
                               chinese_number(ultra_count) +
                               "次都没有失手，干的好！”\n" NOR ;
                }
                
                if (stringp(special))
                {
                        ob1 = new(special);
                        ob1->move(who, 1);
                        tell_object(who, HIM "你获得了一" + ob1->query("unit") + ob1->name() +
                                         HIM "。\n" NOR);
                }
                                              
                message_vision(msg, me, who); 
                if ((ultra_count >= 10 && random(200) == 1) || special) 
                        special_bonus(who); 
                        
                who->set("ultra_count", ultra_count);
                who->delete("ultraquest");

                GIFT_D->delay_work_bonus(who, ([ "exp" : exp, "pot" : pot, "mar" : mar,
                                                 "weiwang" : weiwang, "score" : score ]), 1);                
                destruct(ob); 
                return 1;
        }
                                
        // 只有杀人、寻物、送信/包裹的任务才执行物品接受 
        if (! stringp(type = who->query("ultraquest/quest/type")) 
            || (type != "kill" && type != "find" 
            && type != "give" && type != "send")) 
                return 0; 
        
        // 如果对象为接受信件或包裹的NPC 
        if (me->query("id") == who->query("ultraquest/quest/id")) 
        { 
                switch (type) 
                { 
                case "send" : 
                        if (! ob->is_letter()) 
                                return 0; 
        
                        if (ob->query("send/send") != me->name()) 
                        { 
                                message_vision(CYN "$N" CYN "皱眉道：你会认人么？这" 
                                               "封信又不是写给我的。\n" NOR, me); 
                                return 0; 
                        } 
        
                        if (ob->query("send/user") != who->name()) 
                        { 
                                message_vision(CYN "$N" CYN "皱眉道：这信是谁让你送" 
                                               "的？我怎么不认识。\n" NOR, me); 
                                return 0; 
                        } 
        
                        message_vision(CYN "$N" CYN "看了看" + ob->name() 
                                       + NOR + CYN "，不置可否的点了点头。\n" NOR, me); 
                         
                        rob = new("/clone/misc/receipt"); 
                        rob->set("reply_by", me->query("name")); 
                        rob->set("reply_to", ob->query("send/from")); 
                        rob->set("receive_from", who->query("id"));
                        rob->set("long", "这是一封由" + me->name() + "收到的来信写的回执。\n"); 
                        rob->move(who, 1); 
                        message_vision("$N交给$n一张回执。\n", me, who);                 
                        destruct(ob);
                        
                        break; 
        
                case "give" : 
                        if (! ob->is_bag()) 
                                return 0; 
        
                        if (ob->query("send/send") != me->name()) 
                        { 
                                message_vision(CYN "$N" CYN "皱眉道：你会认人么？这" 
                                               "包裹又不是拿给我的。\n" NOR, me); 
                                return 0; 
                        } 
        
                        if (ob->query("send/user") != who->name()) 
                        { 
                                message_vision(CYN "$N" CYN "皱眉道：这包裹是谁给你" 
                                               "的？还是你自己留着吧。\n" NOR, me); 
                                return 0; 
                        } 
        
                        message_vision(CYN "$N" CYN "看了看" + ob->name() 
                                       + NOR + CYN "，不置可否的点了点头。\n" NOR, me); 
                        rob = new("/clone/misc/receipt"); 
                        rob->set("reply_by", me->query("name")); 
                        rob->set("reply_to", ob->query("send/from")); 
                        rob->set("receive_from", who->query("id"));
                        rob->set("long", "这是一封由" + me->name() + "收到的包裹写的回执。\n"); 
                        rob->move(who, 1); 
                        message_vision("$N交给$n一张回执。\n", me, who);          
                        destruct(ob); 
                        break; 
                } 
                
                who->set("ultraquest/gift/point", 200); 
                who->set("ultraquest/gift/msg", "你通过此番帮助" + 
                                                 me->name() + "的经历"); 
                // give_gift(who);
                return 1; 
        } 
        
        // 如果对象为发布任务的NPC 
        if (who->query("ultraquest/npc5"))  
        {
                npc = who->query("ultraquest/npc5");        
        } else
        if (who->query("ultraquest/npc4"))  
        {
                npc = who->query("ultraquest/npc4");        
        } else
        if (who->query("ultraquest/npc3"))  
        {
                npc = who->query("ultraquest/npc3"); 
        } else
        if (who->query("ultraquest/npc2"))
        { 
                npc = who->query("ultraquest/npc2");    
        } 
        
        if (me->query("id") == npc) 
        { 
                switch (type) 
                { 
                case "kill":
                        if(! ob->is_corpse() && ! ob->is_head())
                                return 0;
                                
                        if (! stringp(ob->query("owner_id"))) 
                        { 
                                message_vision(CYN "$N" CYN "捂着鼻子看了看" + ob->name() 
                                               + NOR + CYN "，道：什么臭烘烘的？快给我" 
                                               "拿走。\n" NOR, me); 
                                return 0; 
                        }
        
                        if (ob->query("killed_by") != who->query("id"))
                                return 0;   
                                             
                        if (ob->query("victim_name") != who->query("ultraquest/quest/name"))
                                return 0;
                                
                        message_vision(CYN "$N" CYN "看了看" + ob->name() 
                                       + NOR + CYN "，不置可否的点了点头。\n" NOR, me); 
                        // destruct(ob); 
                        break;  
                                              
                case "find" : 
                        if (ob->query("id") != who->query("ultraquest/quest/id"))
                                return 0;
                                
                        message_vision(CYN "$N" CYN "看了看" + ob->name() 
                                       + NOR + CYN "，不置可否的点了点头。\n" NOR, me); 
                        // destruct(ob); 
                        break; 
                        
                case "send" :
                        if (! ob->is_receipt())  return 0;
                        message_vision(CYN "$N" CYN "看了看" + ob->name()
                                       + NOR + CYN "，不置可否的点了点头。\n" NOR, me);

                        if (ob->is_receipt()) 
                        { 
                                if (ob->query("reply_to") != me->query("name"))
                                {
                                        message_vision("$N皱眉道：“给我这玩艺干什么？”\n", me);
                                        return 0;
                                }
                                
                                if (ob->query("receive_from") != who->query("id") ||
                                    ob->query("reply_by") != who->query("ultraquest/quest/name"))
                                {
                                        message_vision("$N皱眉道：“这是你帮我送的吗？”\n", me);
                                        return 0;
                                }
                                // destruct(ob);
                        }
                           
                        break;

                case "give" :
                        if (! ob->is_receipt())  return 0;
                        message_vision(CYN "$N" CYN "看了看" + ob->name()
                                       + NOR + CYN "，不置可否的点了点头。\n" NOR, me);
                        if (ob->is_receipt()) 
                        { 
                                if (ob->query("reply_to") != me->query("name"))
                                {
                                        message_vision("$N皱眉道：“给我这玩艺干什么？”\n", me);
                                        return 0;
                                }
                                
                                if (ob->query("receive_from") != who->query("id") ||
                                    ob->query("reply_by") != who->query("ultraquest/quest/name"))
                                {
                                        message_vision("$N皱眉道：“这是你帮我送的吗？”\n", me);
                                        return 0;
                                }
                                // destruct(ob);
                        }
                        break;                        
                } 
                
                destruct(ob);
                who->set("ultraquest/gift/point", 600); 
                who->set("ultraquest/gift/msg", "你通过此番帮助" + 
                                                 me->name() + "的经历"); 
                give_gift(who);
                
                return 1; 
        } 
} 
        
// 完成任务的奖励 
public void give_gift(object me) 
{ 
        int gift; 
        
        // 如果没有注册奖励参数，则中断 
        if (! me->query("ultraquest/gift/msg") 
                || ! me->query("ultraquest/gift/point")) 
                return; 
        
        // 设置连续完成的数量 
        me->add("ultraquest/finish", 1); 
         
        // 给予物品奖励 
        if (me->query("ultraquest/gift/item")) 
                special_bonus(me); 
         
        // 奖励的底数 
        gift = me->query("ultraquest/gift/point") + 
                me->query("ultraquest/finish"); 
        
        GIFT_D->delay_work_bonus(me, ([ "prompt"  : me->query("ultraquest/gift/msg"), 
                                        "exp"     : random(gift) + gift, 
                                        "pot"     : random(gift / 2) + gift / 2, 
                                        "score"   : random(gift / 20) + gift / 20, 
                                        "weiwang" : random(gift / 30) + gift / 30, ])); 
        
        // 取消掉原来的任务属性 
        me->delete("ultraquest/gift"); 
        me->delete("ultraquest/quest"); 
        
        // 提供新的线索，以便任务连贯 
        generate_information(me); 
        return; 
} 
          
// 人物所提供的线索信息 
public void generate_information(object me) 
{
        object ob; 
        string *lines, file; 
        string where, next_npc, next_name, npc_place, place; 
        string npc, npc1, npc2, npc3, npc4, msg, ma; 
        string name;
        int finish;

        // 完成了四个线索后载入本次任务的对象
        if (me->query("ultraquest/npc5"))
        {              
                switch(me->query("ultraquest/type"))
                {
                case "kill":
                        ob = new(CLASS_D("generate") + "/killer1.c");
                        NPC_D->place_npc(ob, ({ "大理一带", "终南山", "关外", "长安城", "杭州城", "西域" }));
                        NPC_D->set_from_me(ob, me, 150); 
                        ob->add_temp("apply/attack", ob->query_skill("force") * 1 / 2);
                        ob->add_temp("apply/dodge", ob->query_skill("force") * 1 / 2);
                        ob->add_temp("apply/parry", ob->query_skill("force") * 1 / 2);
                        ob->add_temp("apply/damage", 1000);
                        ob->add_temp("apply/unarmed_damage", 1000);
                        ob->add_temp("apply/armor", 1000);
                        ob->set_temp("quester", me->query("id"));      
                        
                        me->set("ultraquest/id", ob->query("id"));
                        me->set("ultraquest/name", ob->query("name")); 
                                         
                        NPC_D->random_move(ob);
                        NPC_D->random_move(ob);
                        NPC_D->random_move(ob);
                        
                        place = ob->query("place");
                        
                        tell_object(me, CYN + me->query("ultraquest/name5") + CYN "小声对你说道：" 
                                        NOR + HIR + ob->name() + "(" + ob->query("id") + ")" + NOR + CYN
                                        "就是幕后主人，可千万不要说是我说。\n听说他最近在" + place + "，你快去吧！\n" NOR);                         
                        break;
                case "send":
                        break;
                case "rescue":
                        break;
                case "find":
                        break;
                }
                return;
        }  
                      
        // 以下是给出下一个线索的对象
       if (me->query("ultraquest/npc4"))  
        {
                npc4 = me->query("ultraquest/npc4");
                name = me->query("ultraquest/name4");
                npc = npc4;             
        } else
        if (me->query("ultraquest/npc3"))  
        {
                npc3 = me->query("ultraquest/npc3"); 
                name = me->query("ultraquest/name3");
                npc = npc3;
        } else
        if (me->query("ultraquest/npc2"))
        { 
                npc2 = me->query("ultraquest/npc2");
                name = me->query("ultraquest/name2");
                npc = npc2;       
        } else
        {  
                npc1 = me->query("ultraquest/npc1"); 
                name = me->query("ultraquest/name1");
                npc = npc1;
        }
                                
        // 没有上一个提供线索的人 
        if (! npc) return; 

        ob = new(CLASS_D("generate") + "/cluenpc.c");
        NPC_D->place_npc(ob, ({ "大理一带", "终南山", "关外", "长安城", "杭州城" }));
        NPC_D->set_from_me(ob, me, 100); 
        ob->add_temp("apply/attack", ob->query_skill("force") * 1 / 2);
        ob->add_temp("apply/dodge", ob->query_skill("force") * 1 / 2);
        ob->add_temp("apply/parry", ob->query_skill("force") * 1 / 2);
        ob->add_temp("apply/damage", 500);
        ob->add_temp("apply/unarmed_damage", 500);
        ob->add_temp("apply/armor", 500);
        ob->set_temp("quester", me->query("id"));                              
                                                 
        NPC_D->random_move(ob);

        next_npc = ob->query("id");
        next_name = ob->query("name"); 
        npc_place = ob->query("place");   
                        
        // 记录下下一个线索人物及出现地 
        if (npc4) 
        {
                me->set("ultraquest/npc5", next_npc); 
                me->set("ultraquest/name5", next_name); 
        }
        else
        if (npc3)
        { 
                me->set("ultraquest/npc4", next_npc);
                me->set("ultraquest/name4", next_name);
        }
        else
        if (npc2) 
        {
                me->set("ultraquest/npc3", next_npc);
                me->set("ultraquest/name3", next_name);
        }
        else
        if (npc1) 
        {
                me->set("ultraquest/npc2", next_npc);
                me->set("ultraquest/name2", next_name);
        }
        
        me->set("ultraquest/place", npc_place); 
        
        // 清除上次的线索信息
        me->delete("ultraquest/quest");
        
        // 添加醒目的颜色 
        next_name = HIY + next_name + "(" + next_npc + ")" + NOR + WHT; 

        // 把人称做简单化 
        ma = RANK_D->query_respect(me); 
        
        // 这里增加情节描述
        finish = me->query("ultraquest/finish");
        switch (finish)
        {
        case 1:
                msg = "听说，这个幕后黑手曾遗失过一个重要物品，通过这个物品可以知"
                      "道他是谁。\n好象" + npc_place + "的" + next_name + "就知道这"
                      "个物品的下落。\n" + ma + "可去找他问问，但也不知是真是假";
                break;

        case 2:
                msg = "那物品我到是听说点线索，" + ma + "知道" + npc_place + "吗？\n"
                      "那里有个" + next_name + "，传闻你要找的那东西就在他身上";
                break;

        case 3:
                msg = "不知" + ma + "认识" + npc_place + "的" + next_name + "吗？\n"
                      "你来的真不巧，那个东西我上个月送给他了，你可以找他试试";
                break;

        case 4:
                msg = "很遗憾，你要的东西我前几天搞丢了，不清楚" + ma + "是否认识" 
                      + npc_place + "的" + next_name + "。\n他见过此东西，而且好象知"
                      "道那个物品的主人是谁";
                break;

        default:
                msg = "我想想…" + ma + "去找" + npc_place +
                      "的" + next_name + "问问看吧。\n去年我听"
                      "他说起过这件事";
                break;
        }
        
        // 起始任务的主控人物，可以刷新线索 
        if (name == QUESTER) 
        { 
                if (me->query("ultraquest/begin")) 
                        msg = "…嗯…你去" + npc_place + 
                              "问问" + next_name + "看，他那儿" 
                              "似乎又有些线索"; 
                else 
                        msg = "你可以去" + npc_place + 
                              "找" + next_name + "打听打听，他" 
                              "那似乎有些线索"; 
        } 

        // 客店的北丑，只要有钱就好说话 
        if (name == LURCHER) 
        { 
                switch (random(3)) 
                { 
                case 0: 
                        msg = "…嘿嘿…你去" + npc_place + 
                              "逛逛，那儿有个" + next_name + 
                              "，你问他啥都知道"; 
                        break; 

                case 1: 
                        msg = "…这样吧…你去找" + next_name + 
                              "问问看，那家伙就在" + npc_place + 
                              "，可别走丢了"; 
                        break; 

                default: 
                        msg = "你到" + npc_place + "瞅瞅，如" 
                              "果碰到" + next_name + "那厮，说" 
                              "不定…嘿嘿…能有啥收获"; 
                        break; 
                } 
        } 

        // 提供出线索信息 
        message("vision", name + "悄悄在" + me->name() + "耳" 
                "边说了些什么。\n", environment(me), 
                ({ me })); 
 
        // 提供出任务的描述信息 
        msg = WHT + name + WHT "悄悄对你说道：" + msg + "。\n" NOR; 

        // tell_object(me, sort_msg(msg)); 
        tell_object(me, msg);

        return; 
} 

