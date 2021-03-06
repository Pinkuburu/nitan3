// This is player's own perform (Write by Lonely@nt2)
// Create by 段左助(Rare) at Sat Apr  8 14:47:07 2006
// 六脉龙气(dragon)

#include <ansi.h>
#include <combat.h>

#define PFM "「" HIW "六脉龙气" NOR "」"

inherit F_SSERVER;
#include <balance.h>

string *weapon_sk = ({
        "sword", "blade", "staff", "whip", "club", "hammer", "axe"
});

int perform(object me, object target)
{
        object weapon;
        string msg;
        int ap, dp;
        int damage;
        int attack;

        if (! me->query("can_perform/" + "six-finger" + "/" + "dragon"))
                return notify_fail("你还没有受到高手指点，还不会运用" PFM "。\n");

        if (! target)
        {
                me->clean_up_enemy();
                target = me->select_opponent();
        }

        if (! target || ! me->is_fighting(target))
                return notify_fail(PFM "只能对战斗中的对手使用。\n");

        if (member_array("finger", weapon_sk) != -1)
        {
                attack = WEAPON_ATTACK;
                if (! objectp(weapon = me->query_temp("weapon"))
                   || (string)weapon->query("skill_type") != "finger")
                        return notify_fail("你所使用的武器不对，难以施展" PFM "。\n");
        } else
        {
                attack = UNARMED_ATTACK;
                if (me->query_temp("weapon") || me->query_temp("secondary_weapon"))
                        return notify_fail(PFM "只能空手使用。\n");             
        }
        
        if ((int)me->query_skill("six-finger", 1) < 400)
                return notify_fail("你" + to_chinese("six-finger") + "不够娴熟，难以施展" PFM "。\n");

        if (member_array("finger", weapon_sk) == -1)
        {
                if (me->query_skill_prepared("finger") != "six-finger")
                        return notify_fail("你没有准备" + to_chinese("six-finger") + "，难以施展" PFM "。\n");
        }
        
        if (me->query("neili") < 300)
                return notify_fail("你现在的真气不够，难以施展" PFM "。\n");

        if (! living(target))
                return notify_fail("对方都已经这样了，用不着这么费力吧？\n");

        msg = HIW "$N眼中神光湛湛，双手平伸，六大穴之中各自涌出一束剑气，随即$N双手一合，将六束剑气困于掌中，剑气激荡冲撞不止。至气势最盛之时$N双手再分，喝道：“阿录给” 六道剑气之合竟如狂龙，狂冲$n而去！" + "\n" + NOR;

        ap = ap_power(me, "finger");
        dp = dp_power(target, "parry");
         
        if (ap * 2 / 3 + random(ap * 2) > dp)
        {
                damage = da_power(me, "finger") * 3;
                msg += COMBAT_D->do_damage(me, target, attack, damage, 100, HIM "$n眼见狂龙袭来，避无可避，躲无可躲，只得闭目受死。刹那间被龙气吞没，软瘫在地。" + "\n" NOR);
                me->add("neili", -200);
                me->start_busy(1);
                if (! target->is_busy())
                        target->start_busy(2);
        } else
        {
                msg += NOR + CYN "$p见狂龙袭来，竟无畏惧，侧身间，一把抓住龙颈，掌中使力之下，那龙气竟就此散碎了。" + "\n" NOR;
                me->add("neili", -100);
                me->start_busy(2);
        }
                
        message_sort(msg, me, target);
        return 1;
}

