// /d/playertown/road3.c ��ʯС��
// Build 1999.12.10 bmw

inherit BUILD_ROOM;

void create()
{
	set("short","��ʯС��");
	set("long",
"����һ������ʯ�ӵ�С·�����߳��ż��Ÿߴ������������С·�ı�\n"
"������������С����ߺ��������ż��о�������ɣ���������ͨ�������\n"
"·�������С·�������ׯ�����ģ���\n"
);

	set("recordable_room",1);
	set("outdoors","playertown");
	set("exits",([
		"north"	:	__DIR__"road2",
		"east"	:	__DIR__"road4",
//        "enter"  :   "/data/room/skills/xiaoyuan",
		"west"	:	__DIR__"road16",
	]));
	
	setup();
	
	// replace_program(ROOM);
	
}