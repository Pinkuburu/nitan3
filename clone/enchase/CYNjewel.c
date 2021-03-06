#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(CYN "宝石" NOR, ({ "CYN jewel","jewel" }));
        set_weight(30);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", CYN "一块闪烁这奇异光芒的青色宝石。\n" NOR);
                set("unit", "块");
                set("value", 100000);
                set("no_identify", 1);
                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                    
                set("enchase/type", "all");   // 可镶嵌的道具类型"all"或"sword"或"blade"或"cloth"等
                set("enchase/cur_firm", 90);  // 抗磨损度
                set("enchase/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                        "kar" : 5,           // 增加福源

                        ////////////////////     兵器特有属性

                       "attack" : 50,       // 攻击等级：+50
                        "xuruo_status"  : 2,  // 战力削弱(虚弱)2秒
                        "suck_jingli"   : 12, // 吸取精力12

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "kar" : 5,           // 增加福源

                        ////////////////////     防具特有属性

                        "avoid_poison"  : 50, // 毒性回避：50%
                        "dodge"  : 50,       // 轻功等级：+50
                        "reduce_busy"   : 25, // 化解忙乱：25%

                        "neili_recover_speed"  : 10, // 内力恢复速度：+10%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}

