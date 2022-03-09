typedef string str_t<255>;

struct input {
	str_t path;
	int a;
	int b;
};

program PARTB_PROG {
	version PARTB_VERS {
		string partb(input)=1;
	}=1;
}=0x12345678;