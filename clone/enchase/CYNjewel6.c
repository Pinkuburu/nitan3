#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(CYN "魔之宝石" NOR, ({ "CYN jewel6","jewel6" }));
        set_weight(40);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", CYN "一块闪烁这奇异光芒的青色宝石，"
                    "让人感到一阵阵的心悸。\n" NOR);
                set("unit", "块");
                set("value", 180000);
                set("no_identify", 1);
                set("wear_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "戴在头上。\n" NOR);
                set("remove_msg", HIC "$N" HIC "轻轻地把$n"
                    HIC "从头上摘了下来。\n" NOR);
                    
                set("enchase/SN", 3);
                set("enchase/type", "all");   // 可镶嵌的道具类型"all"或"sword"或"blade"或"cloth"等
                set("enchase/cur_firm", 90);  // 抗磨损度
                set("enchase/wspe_data",      // 兵器上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性


                        "kar" : 8,           // 增加福源

                        ////////////////////     兵器特有属性

                       "attack" : 80,       // 攻击等级：+80
                        "xuruo_status"  : 3,  // 战力削弱(虚弱)3秒
                        "suck_jingli"   : 24, // 吸取精力24

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "kar" : 8,           // 增加福源

                        ////////////////////     防具特有属性

                        "avoid_poison"  : 75, // 毒性回避：75%
                        "dodge"  : 80,       // 轻功等级：+80
                        "reduce_busy"   : 40, // 化解忙乱：40%

                        "neili_recover_speed"  : 15, // 内力恢复速度：+15%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}

