#include <ansi.h>
inherit ITEM;
// by dream
int is_enchase_ob() { return 1; }
void create()
{
        set_name(YEL "圣之宝石" NOR, ({ "YEL jewel5","jewel5" }));
        set_weight(30);
        if (clonep())
                set_default_object(__FILE__);
        else {
                set("long", YEL "一块闪烁这奇异光芒的黄色宝石，"
                    "让人感到一阵阵的心悸。\n" NOR);
                set("unit", "块");
                set("value", 160000);
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


                        "con" : 6,           // 增加身法

                        ////////////////////     兵器特有属性

                        "busy_time"     : 3,  // 忙乱敌人3秒
                        "unarmed_damage": 60, // 手法伤害：+60
                        "no_exert"      : 3,  // 内息紊乱3秒

                   ]));

                set("enchase/aspe_data",       // 防具上附加属性汇总
                   ([
                        ////////////////////     兵防共有属性

                        "con" : 6,           // 增加身法

                        ////////////////////     防具特有属性

                        "avoid_poison"  : 60, // 毒性回避：60%
                        "dodge"  : 60,       // 轻功等级：+60
                        "reduce_busy"   : 35, // 化解忙乱：35%

                        "jingli_recover_speed" : 13, // 精力恢复速度：+13%
                   ]));
        }
        setup();
}

int query_autoload()
{
        return 1;
}


