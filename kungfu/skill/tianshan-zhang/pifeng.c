// Code of ShenZhou
//pifeng ������
//by wsky, 20/08/00

#include <ansi.h>

inherit F_CLEAN_UP;
inherit F_DBASE;
inherit F_SSERVER;

int remove_effect(object me,int skill);

int second_hit(object me, object target, int skill);

int perform(object me, object target)
{
        string weapon, str;
        int skill, bonus, damage;
        mapping myfam;
        object the_weapon;

        if( !target ) target = offensive_target(me);

        if ( (!( myfam= me->query("family")) || myfam["family_name"] != "������") )
                  return notify_fail("ֻ�����޵������ñ����ش��ڹ�����ʹ�������ȷ���\n");

        if( !target
        ||      !target->is_character()
        ||      !me->is_fighting(target) )
                return notify_fail("�����ȷ�ֻ�ܶ�ս���еĶ���ʹ�á�\n");

        if( !objectp(the_weapon = me->query_temp("weapon")) )
                return notify_fail("�����Ҳ�����ȷ���\n");

        if( the_weapon->query("skill_type") != "staff")
                return notify_fail("��ʹ�õı������ԣ�\n");

        if ( me->query_temp("sucked") )
                return notify_fail("��ո���ȡ���������޷�ʩչ�����ȷ���\n");

        if( me->query_temp("pifeng",1))
                return notify_fail("������ʹ�������ȷ���\n");

        if( me->query_skill_mapped("force") != "huagong-dafa" )
                return notify_fail("�����õĲ��ǻ����󷨣��޷�ʩչ�����ȷ���\n");

        if( me->query_skill("huagong-dafa", 1) < 200 )
                return notify_fail("��Ļ����󷨻��δ�����޷�ʩչ�����ȷ���\n");

        if( me->query_skill_mapped("parry") != "tianshan-zhang" )
                return notify_fail("��û�н���ɽ�ȷ��������м��У��޷�ʩչ�����ȷ���\n");

        if( me->query_skill("tianshan-zhang",1) < 200 )
                return notify_fail("�����ɽ�ȷ���Ϊ���㣬������ʹ�������ȷ���\n");

        skill =  ( me->query_skill("tianshan-zhang",1)
                + me->query_skill("force") ) / 5;

        if( me->query("neili") <= skill*3 )
                return notify_fail("�����������ʹ�������ȷ���\n");

        if( me->query("jingli") <= skill )
                return notify_fail("��ľ�������ʹ�������ȷ���\n");

        if( me->query("jing") <= skill )
                return notify_fail("��ľ�����ʹ�������ȷ���\n");

        weapon = me->query_temp("weapon");

        skill = me->query_skill("parry",1) + me->query_skill("tianshan-zhang",1);
        
        skill = skill/2 + random(skill/2);

        me->add_temp("apply/parry", skill/2);
        me->add_temp("apply/dodge", skill/2);
        me->add_temp("apply/defence", skill/2);
        me->add_temp("apply/attack", skill/2);       
        
        message_vision(HIW "\n\n$Nս������������һ�ڣ����һ������"+(me->query_temp("weapon"))->query("name")+HIW"�߸߾���ʹ����$n����һ�С�\n\n"NOR, me, target);
        
        me->add_temp("apply/attack", skill/2);       
        me->add_temp("apply/damage", skill/2);
        
        COMBAT_D->do_attack(me, target, me->query_temp("weapon"));

        me->add("neili", -150);
        me->add("jingli", -50);
        
        me->add_temp("apply/attack", -(skill/2));
        me->add_temp("apply/damage", -(skill/2));
        
        if (me->query("tianshan")==1) {
        
                me->add("neili", -350);
                me->add("jingli", -250);
                
                message_vision(HIW "\n\n$Nһ�в��أ�����������"+(me->query_temp("weapon"))->query("name")+HIW"�������߶����ֳ�ҫ�۹�â��\n\n"NOR, me, target);
                message_vision(HIB "$N��$n��������֮�ʣ�һ����������������$n���ؿڰ�ȥ��\n"NOR,me,target);
                if((me->query("combat_exp")/2)>random(target->query("combat_exp"))){
                        message_vision(HIR "\n$nһ���ƺߣ��ؿ����ƣ�ͬʱ������й������\n"NOR,me,target);                           
                        damage=skill+random(skill*2);
                        target->receive_wound("qi",damage,me);
                        target->add("neili", -me->query_skill("force",1)*(3+random(3)));
                        me->add("neili", random(me->query_skill("force",1)*(3+random(3))));
                        
                        str = COMBAT_D->status_msg((int)target->query("qi") * 100 /(int)target->query("max_qi"));
                        message_vision("($N"+str+")\n", target);
                } else {
                        message_vision(HIY "\n$n����������թ��æ���Ծ����\n"NOR,me,target);                               
                }
        
                call_out("second_hit", 3+random(5), me, target,skill);
        }
             
        if (target)
        message_vision(HIY "\n\n$N�������˺�һ����������ʹ�û������磬�����ػ�סȫ�����¸���Ҫ����\n\n" NOR, me, target);
        
        me->set_temp("pifeng",1);
        
        me->start_call_out( (: call_other, __FILE__, "remove_effect", me, skill :), me->query_skill("tianshan-zhang",1)/10);
        return 1;
}

int second_hit(object me, object target,int skill){
        int damage;
        string str;

        if (!me) return 1;

        if (!living(me) || !target || !me->is_fighting(target))
        {
                remove_effect(me,skill);
                return 1;
        }
        
        if (me->query_temp("pifeng")==1 && me->is_fighting()){
        
                me->add("neili", -100);
                me->add("jingli", -50);
                
                message_vision(HIY "\n\n$Nǳ�����������������дʣ�ͻȻ���Ƚ����֣���������$n��ȥ��\n"NOR, me, target);
     
                if((me->query("combat_exp")*2/3)>random(target->query("combat_exp"))){
                        message_vision(HIR "\n$nһ���ƺߣ��ؿ����ƣ�ͬʱ������й������\n"NOR,me,target);                           
                        damage=skill/2+random(skill);
                        target->receive_wound("qi",damage,me); 
                        target->add("neili", -me->query_skill("force",1)*(2+random(2)));
                        me->add("neili", random(me->query_skill("force",1)*(3+random(2))));
                        str = COMBAT_D->status_msg((int)target->query("qi") * 100 /(int)target->query("max_qi"));
                        message_vision("($N"+str+")\n", target);
                } else {
                        message_vision(HIY "\n$n����������թ��æ���Ծ����\n"NOR,me,target);                               
                }
                
                call_out("second_hit", 5+random(5), me, target,skill);
        }

        return 1;
}

int remove_effect(object me, int skill)
{
        if (!me) return 0;

        if (!me->query_temp("pifeng")) return 0;

        me->add_temp("apply/parry", -skill/2);
        me->add_temp("apply/attack", -skill/2);
        me->add_temp("apply/dodge", -skill/2);
        me->add_temp("apply/defence", -skill/2);

        me->delete_temp("pifeng");

        if(me->is_fighting()){
                message_vision(HIY "\n$N΢��ƣ����ֻ�ý��������е��ȷ�����������\n"NOR, me);
        } else message_vision(HIY "\n$N����������һ�������������ջء�\n"NOR, me);
        return 0;
}
