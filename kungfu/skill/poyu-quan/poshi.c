// poshi.c 破石一击

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me, object target)
{
	object weapon;
	int damage;
	string msg;
        int ap, dp;

        if (! target)
        {
	        me->clean_up_enemy();
	        target = me->select_opponent();
        }
	if (! target || ! me->is_fighting(target))
		return notify_fail("「破石一击」只能对战斗中的对手使用。\n");

	if (me->query_temp("weapon"))
		return notify_fail("只有空手才能使用破玉拳。\n");

	if ((int)me->query_skill("poyu-quan", 1) < 120)
		return notify_fail("你的破玉拳不够娴熟，不会使用「破石一击」。\n");

        if ((int)me->query_skill("zixia-shengong",1) < 120)
                return notify_fail("你的紫霞神功修为还不够。\n");
	                        
	if ((int)me->query_skill("force") < 150)
		return notify_fail("你的内功修为还不够高，难以运用「破石一击」。\n");
			
	if ((int)me->query("neili") < 300)
		return notify_fail("你现在真气不够，不能使用「破石一击」。\n");
			
	msg = HIY "$N" HIY "大喝一声，握紧的拳头蒙上一层淡淡的紫色雾气，闪电般重重的击向$n" HIY "。\n"NOR;

       ap = me->query_skill("force") + me->query("str") * 10;
       dp = target->query_skill("force") + target->query("str") * 10;

        if (ap / 2 + random(ap) > dp || !living(target))
	{
                damage = ap + (int)me->query_skill("cuff");
                damage = damage / 3;
                damage += random(damage);

		me->add("neili", -200);
              msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 40,
		                           HIR "只见$P" HIR "的紫拳如同硬玉般砸得$p" HIR
                                           "飞了出去，重重的摔在地上，吐血不止！\n" NOR);
		me->start_busy(2);
                target->start_busy(1+random(2));
	} else 
	{
		msg += HIC "可是$p" HIC "奋力招架，硬生生的挡开了$P"
                       HIC "这一招。\n"NOR;
		me->add("neili", -100);
		me->start_busy(3);
	}
	message_combatd(msg, me, target);

	return 1;
}
