// Copyright (C) 2003, by Lonely. All rights reserved.
// This software can not be used, copied, or modified 
// in any form without the written permission from authors.
// hun.c ��ػ�Ԫ
 
#include <ansi.h>
#include <combat.h>

#define WU "��" HIW "��ػ�Ԫ" NOR "��"
 
inherit F_SSERVER;
#include <balance.h>
 
int perform(object me, object target)
{
        string msg;
        object weapon;
        int skill, ap, dp, damage;

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (userp(me) && ! me->query("can_perform/pili-shou/hun"))
                return notify_fail("�㻹û���ܹ�����ָ�㣬�޷�ʩչ" WU "��\n"); 

        if (! me->is_fighting(target))
                return notify_fail(WU "ֻ�ܶ�ս���еĶ���ʹ�á�\n");
 
        skill = me->query_skill("pili-shou", 1);

        if (skill < 140)
                return notify_fail("��Ļ�Ԫ�����ֵȼ�����, ����ʩչ" WU "��\n");

        if (me->query_skill("force", 1) < 160)
                return notify_fail("���ڹ���Ϊ�������޷�ʩչ" WU "��\n");

        if (me->query("max_neili") < 2400)
                return notify_fail("��������Ϊ���㣬�޷�ʩչ" WU "��\n");

        if (me->query("neili") < 340)
                return notify_fail("��������������޷�ʩչ" WU "��\n");
 
        if (me->query_skill_prepared("hand") != "pili-shou")
                return notify_fail("��û��׼��ʹ�û�Ԫ�����֣��޷�ʩչ" WU "��\n");

       if (! living(target))
              return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

        msg = HIC "$N" HIC "ŭ��һ����ʩ�����С�" HIW "��ػ�Ԫ" HIC "����˫�ƽ���"
              "Ϯ��$n" HIC "���Ʒ��������������з���֮�ơ�\n" NOR;
 
        ap = ap_power(me, "hand") +
             me->query_skill("force");

        dp = dp_power(target, "parry") +
             target->query_skill("force");
             
        if (ap / 2 + random(ap) > dp)
        {
                me->add("neili", -300);
                damage = da_power(me, "hand");
                msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 70,
                                           HIR "$n" HIR "ֻ������ǰһ���ʹ�����ۡ���һ"
                                           "�����һ����Ѫ��\n" NOR);
                me->start_busy(1 + random(2));
        } else
        {
                me->add("neili", -200);
                msg += CYN "ֻ��$n" CYN "���Ų�æ������һ�������"
                       "��$N" CYN "�ı�ɱһ����\n" NOR;
                me->start_busy(3);
        }
        message_combatd(msg, me, target);

        return 1;
}

