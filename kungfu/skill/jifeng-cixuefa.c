inherit SKILL;

mapping *action = ({
([      "action":"$N����һչ��ʩ��һ�С�Ыβ�롹������$w��������$n��$l",
        "force" : 30,
        "attack": 15,
        "dodge" : 10,
        "parry" : 12,
        "damage": 25,
        "lvl"   : 0,
        "skill_name" : "Ыβ��",
        "damage_type" : "����"
]),
([      "action":"$Nһ��߳�ȣ�$w���������£�ʩһ�С���ü�̡���$n��$l��ȥ",
        "force" : 53,
        "attack": 21,
        "dodge" : 12,
        "parry" : 13,
        "damage": 37,
        "lvl"   : 10,
        "skill_name" : "��ü��",
        "damage_type" : "����"
]),
([      "action":"$N����һԾ��������$wʹ������⾢������������������$n$l",
        "force" : 71,
        "attack": 24,
        "dodge" : 15,
        "parry" : 22,
        "damage": 45,
        "lvl"   : 20,
        "skill_name" : "��⾢",
        "damage_type" : "����"
]),
([      "action":"$N����$w�����һ�Σ���Ȼʹ��һ�С��ط������ơ���쬵Ĵ���$n$l",
        "force" : 98,
        "attack": 35,
        "dodge" : 15,
        "parry" : 13,
        "damage": 54,
        "lvl"   : 40,
        "skill_name" : "�ط�������",
        "damage_type" : "����"
]),
([      "action":"$N����Ծ��һʽ����ת�����ơ�������$w�����Ẉ̂���������$n��ȥ",
        "force" : 140,
        "attack": 46,
        "dodge" : 11,
        "parry" : 9,
        "damage": 65,
        "lvl"   : 60,
        "skill_name" : "��ת������",
        "damage_type" : "����"
]),
});


int valid_enable(string usage)
{
	return usage == "dagger" || usage == "parry";
}

int valid_learn(object me)
{
        if ((int)me->query("max_neili") < 150)
                return notify_fail("�������������\n");

        if ((int)me->query_skill("force") < 30)
                return notify_fail("����ڹ����̫ǳ��\n");

        if ((int)me->query_skill("dagger", 1) < (int)me->query_skill("jifeng-cixuefa", 1))
                return notify_fail("��Ļ����̱�ˮƽ���ޣ��޷���������ļ����Ѩ����\n");

    return 1;
}

string query_skill_name(int level)
{
        int i;

        for(i = sizeof(action) - 1; i >= 0; i--)
                if(level >= action[i]["lvl"])
                        return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
        int i, level;

        level = (int) me->query_skill("jifeng-cixuefa", 1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
        object weapon;

        if (! objectp(weapon = me->query_temp("weapon")) ||
            (string)weapon->query("skill_type") != "dagger")
                return notify_fail("�����ֵ������޷���ϰ�����Ѩ����\n");

        if ((int)me->query("qi") < 70)
                return notify_fail("������������������Ѩ����\n");

        if ((int)me->query("neili") < 70)
                return notify_fail("������������������Ѩ����\n");

        me->receive_damage("qi", 65);
        me->add("neili", -62);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"jifeng-cixuefa/" + action;
}