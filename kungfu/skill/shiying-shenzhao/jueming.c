// jueming.c 绝命

#include <ansi.h>
#include <combat.h>

inherit F_SSERVER;

int perform(object me)
{
        string msg;
        object weapon, target;
        int ap, dp, damage;

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (userp(me) && ! me->query("can_perform/shiying-shenzhao/jueming"))
                return notify_fail("你还不会使用「绝命」。\n");

        if (! target || ! me->is_fighting(target))
                return notify_fail("「绝命」只能对战斗中的对手使用。\n");

        if (me->query_temp("weapon"))
                return notify_fail("你只能空手使用「绝命」。\n");

        ap = me->query_skill("shiying-shenzhao", 1);

        if (ap < 150)
                return notify_fail("你的弑鹰神爪等级不够, 无法使用「绝命」！\n");

        if (me->query("neili") < 180)
                return notify_fail("你的真气不够，无法运用「绝命」！\n");

        msg = HIR "$N" HIR "一声暴喝，手臂上青筋突起，左手一晃，右手成爪，向$n" HIR
              "前胸迅疾抓到。\n" NOR;

        dp = target->query_skill("parry", 1);
        if (dp < 1)  dp = 1;
        if (ap / 2 + random(ap) > dp)
        {
                me->add("neili", -150);
                damage = ap + (int)me->query_skill("force", 1);
                damage += random(damage / 2);
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 60,
                                           HIR "$n" HIR "闪避不及，前胸顿时被抓出"
                                           "五条血痕，不禁长声惨呼！\n" NOR);
                me->start_busy(2 + random(3));
        } else
        {
                me->add("neili", -50);
                damage = ap / 2 + random(ap / 2);
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 30,
                                           HIM "$n" HIM "不敢小觑此招，拼力拆解，"
                                           "但双臂仍被抓出了几条血印！\n" NOR);
                me->start_busy(3);
        }
        message_combatd(msg, me, target);
        return 1;
}

