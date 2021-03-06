// tao.c 太极拳「套」字诀

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me, object target)
{
        int damage;
        string msg;

        if (! target) target = offensive_target(me);

/*
        if (userp(me) && ! me->query("can_perform/taiji-quan/tao")) 
                return notify_fail("你还不会使用「套」字诀。\n"); 
*/

        if (! target || ! me->is_fighting(target))
                return notify_fail("「套」字诀只能对战斗中的对手使用。\n");

        if (objectp(me->query_temp("weapon")))
                return notify_fail("你必须空手才能使用「套」字诀！\n");         
                
        if ((int)me->query_skill("taiji-quan", 1) < 300)
                return notify_fail("你对太极拳理的理解还不够，不会使用「套」字诀。\n");
                                
        if ((int)me->query("neili", 1) < 200)
                return notify_fail("你现在真气太弱，不能使用「套」字诀。\n");
                        
        msg = HIY "$N" HIY "使出太极拳「套」字诀，左手划圈，右拳含劲，向$n"
              HIY "骤然打出。\n" NOR;
        me->add("neili", -500);

        if (random(me->query_skill("force")) > target->query_skill("force") / 2 || !living(target))
        {
                me->start_busy(3);
                target->start_busy(random(3));
                damage = ((int)me->query_skill("force") + 
                         (int)me->query_skill("cuff"))/2;
                damage += random(damage);
                damage += me->query("jiali");

                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK,
                                           damage, 50,
                                           HIR "$n" HIR "被$N" HIR "左手所扰，只感双臂"
                                           HIR "难以施展，被$N" HIR "右拳狠狠击中！\n"
                                           ":震伤@?");
        } else 
        {
                me->start_busy(3);
                msg += CYN "可是$p" CYN "看破了$P"
                       CYN "的企图，不敢硬接这一招，向后闪开。\n" NOR;
        }
        message_combatd(msg, me, target);
        return 1;
}

