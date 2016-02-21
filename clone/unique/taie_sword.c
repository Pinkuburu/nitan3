// Copyright (C) 2003, by Xlz. All rights reserved.   
// This software can not be used, copied, or modified    
// in any form without the written permission from authors.   
// tai-sword.c ̩
// for only one object in mud  

#include <ansi.h>   
#include <weapon.h> 
inherit SWORD;   
inherit F_UNIQUE; 
void create()    
{
        set_name(CYN "̩����" NOR, ({"taie sword", "taie", "sword"})); 
        set_weight(15000); 
        if (clonep())   
                set_default_object(__FILE__);    
        else { 
                set("unit", "��");    
                set("long", sort_msg(CYN"�����Ķ����ѱ������ı���Χ�������ꡣ��������������" 
                                     "����õ����������֮�������������˶�˵��̩������ŷұ��" 
                                     "�͸ɽ�����ʦ����������������λ��ʦȴ��������Ϊ������" 
                                     "˵̩������һ���������֮�����Ѵ��ڣ�ֻ�����Ρ��޼�����" 
                                     "�ǽ������Ѵ������֮�䣬ֻ�ȴ�ʱ��������������ʱ������" 
                                     "���˺�������һ���˽����ɡ�������ʱ��Ϊǿ�󣬽�����Ȼ��" 
                                     "Ϊ�Լ������ʸ�õ���ѱ�������������ԸΥ���˽�ȴ����С" 
                                     "�Ĺ����ɣ�����֮ʱ��������Ȼ��Ȼ�Կ�׭�塰̩�������֣�" 
                                     "�ɼ�ŷұ���ɽ����Բ��������Ȼ�ʲ�������������������" 
                                     "�����������ܾ������ǽ�������������������֮����������" 
                                     "�����������⣬�����󲿷ֳǳغܿ����䲢�Ҷ���Ҳ������Χ" 
                                     "ס��һ�����ꡣ�������ݸ����������޴棬Σ�ڵ�Ϧ����һ��" 
                                     "����������ʹ�߷������ͨ뺣����ٲ����������콫���ݴ˳�" 
                                     "����ʱ��ʯ��٣������������Ը����ң������Լ�Ҫ���ϳ�ͷ" 
                                     "ս�У�������ƣ��Լ����ô˽����أ�Ȼ������Ҫʰ�ô˽���" 
                                     "�����������������˽�Ͷ����ף���̩���������������ڶ�" 
                                     "��������������ϳ�ͷ��ֻ�������������������գ��Լ���" 
                                     "������������֮�е�һҶ���ۣ���ʱ������Σ�ա�����������" 
                                     "ʼ���ǣ��ź���ͬɽ����Х�������ڼ�������˫����������̾" 
                                     "һ����̩��������̩�������ҽ��콫���Լ�����Ѫ�����㣡��" 
                                     "�ǣ��ν����ʣ�����ֱָ�о���������˼���漣�����ˣ�ֻ��" 
                                     "һ�Ѱ��罣�����������������ʱ��ɰ��ʯ������գ�������" 
                                     "���������У������������ҡ�Ƭ��֮������͵أ���Ѫǧ��" 
                                     "��ȫ����û���������󣬳��������������߷�����ʵ���̩" 
                                     "����Ϊ�λ������֮��������ӶԳ�������̩������һ������" 
                                     "֮����������֮���������������������澳���䲻����������" 
                                     "֮��׿Խ���֣����Ǵ���������֮��������̩�����Ľ���֮��" 
                                     "�������ڡ�Խ���顷��̩������һ������֮����\n"NOR));
        set("value", 100000);   
        set("material", "steel");   
        set("no_sell", "�ҵ��졭�㡭��֪������ɶô������Ҳ����������");   
        set("unique", 1);   
        set("rigidity", 500);
        set("replica_ob", "/clone/weapon/changjian");   
        set("no_put",1); 
        set("no_steal",1);  
        set("wield_msg",CYN"$N[��]��һ�����һ��̩������ֻ��һ�Ѱ��罣�������������ʱ����ɰ��ʯ������գ�"
                              "���������������С�\n"NOR);
        set("unwield_msg",CYN"$N�����е�̩�������뽣�ʣ���ջָ���ƽ����\n"NOR);  
        } 
        init_sword(850);
        setup(); 
}
mixed hit_ob(object me, object victim, int damage_bonus)  
{
        victim->receive_damage("jing", me->query("str") * 2, me); 
        victim->receive_wound("jing", me->query("str") * 2, me);  
        return CYN "̩����[��]һ���죬����һ������ɰ��ʯ������գ�"   
              CYN "$n�ŵĵ�ͷ���ܡ�\n" NOR;  
} 