// dan.c 落日

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        object weapon,weapon2;
        int damage,i;
        string msg,str;
        int ap, dp;

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }
        
        if (! target || ! me->is_fighting(target))
                return notify_fail("「落日」只能对战斗中的对手使用。\n");
 
        if(me->query_skill_mapped("cuff") != "luorishenquan-cuff") 
                return notify_fail("你没有用落日神拳，无法使用「落日」绝招！\n");

        if (me->query_skill_prepared("cuff") != "luorishenquan-cuff")
                return notify_fail("你没有准备使用落日神拳，无法施展「落日」绝招。\n");

        if (objectp(weapon = me->query_temp("weapon")) ||
            objectp(weapon = me->query_temp("handing")) )
                return notify_fail("你必须空着双手才能使用拳法绝招。\n");

        if ((int)me->query_skill("luorishenquan-cuff", 1) < 200)
                return notify_fail("你的落日神拳不够娴熟，不会使用「落日」。\n");
                                
        if ((int)me->query_skill("force") < 300)
                return notify_fail("你的内功修为不够高。\n");
                        
        if( (int)me->query("neili") < 800 )
                return notify_fail("你现在内力太弱，不能使用「落日」。\n");
                        
        msg = HIY "\n$N" HIY "仰天发出一声长笑，使出落日神拳的绝招［落日］，\n"
              "就似灿烂无比的夕阳，顿时间天地亦为之变色！\n" NOR;

        ap = me->query_skill("cuff") + me->query_skill("force");
        ap += me->query("jiali");
        ap += me->query("jianu");
        
        damage = ap*2 + random(ap);
        damage += me->query("jiali");

        if (living(target))
                dp = target->query_skill("dodge",1) + target->query_skill("parry",1);
        else
                dp = 0;

        dp = dp*3/2;

        weapon2 = target->query_temp("weapon");
        
        if (ap / 2 + random(ap) > dp)
        {
                me->add("neili", -damage/2);

                msg += HIR "\n只听见啪的一声巨响，$N" HIR "的双拳已经击中了$n" HIR "！\n" NOR;
                target->receive_damage("qi",damage,me);
                target->receive_wound("qi",damage/2 + random(damage/2),me);
		str = COMBAT_D->status_msg((int)target->query("qi") * 100 /(int)target->query("max_qi"));
		msg += "($n"+str+")\n";
                
                target->start_busy(1+random(2));
                me->start_busy(3+random(2));
        } else 
        {
                msg += HIY "$n" HIY "轻轻一闪，避开了$N" HIY "的这一招。\n" NOR;

                me->add("neili", -100);
                me->start_busy(3);
                if (target->is_busy())
                        target->start_busy(1);
        }
        message_combatd(msg, me, target);

        return 1;
}
