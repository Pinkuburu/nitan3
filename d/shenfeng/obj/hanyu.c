// medicine: hanyu.c
// by mei 
#include <ansi.h>
inherit ITEM;
void setup()
{}
void create()
{
        set_name(HIG "寒玉" NOR, ({ "han yu", "yu" }));
        set_weight(100000);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("unit", "块");
                set("long", "这是一块只有在极寒冷的地方才能长成的、通体晶莹碧绿的千年"+HIG+"寒玉"+NOR+"。\n");
                set("value", 1000000);
        }
        setup();
}
int move(mixed dest, int silently)
{
    if (objectp(dest)){
        if (userp(dest))
         dest->apply_condition("xuecan_poison", 100);
    }
    return ::move(dest,silently);
}

