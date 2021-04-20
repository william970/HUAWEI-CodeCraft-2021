#include<iostream>
#include<fstream>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include<assert.h>
#include<map>
#include<functional>
#include<vector>
#include<memory>
#include<algorithm>
#include<list>
#include<thread>
using namespace std;
#define MIN_DIFF 0.4
#define BILI 0.3
/*LogicalOperation*/
class LogicalOperation {
public:
	LogicalOperation(string& opname, string& xinghao, string& id);
	LogicalOperation(string& opname, string& id);
	string op_name;
	string xing_hao;
	string xuniji_id;
};
LogicalOperation::LogicalOperation(string& opname, string& xinghao, string& id) {
	this->op_name = opname;
	this->xuniji_id = id;
	this->xing_hao = xinghao;
}
LogicalOperation::LogicalOperation(string& opname, string& id) {
	this->op_name = opname;
	this->xuniji_id = id;
	this->xing_hao = "unkown";
}
/*end LogicalOperation*/
/*xuniji*/
class XUNIJI
{
public:
	XUNIJI(string &xinghao, int core, int memory, bool shuangjiedian);
	string xinghao;
	int neihe;
	int neicun;
	bool shuangjiedian;
	int getState(int current_day);//0 还未add，1 当前正在服务器上运行，2已经释放
	int start_day;//add的当前天
	int continue_day; //将会持续的时间
	int end_day;//释放的日期
	int state;//当前状态，0 还未add，1 当前正在服务器上运行，2已经释放
	bool isSmall;//记录是否是小的虚拟机
	XUNIJI() = default;
};
XUNIJI::XUNIJI(string &xinghao, int core, int memory, bool shuangjiedian) {
	this->xinghao = xinghao;
	this->neihe = core;
	this->neicun = memory;
	this->shuangjiedian = shuangjiedian;
}
int XUNIJI::getState(int current_day) {
	if (current_day < this->start_day) {
		return 0; //0表示还未add
	}
	else if (current_day < this->start_day + this->continue_day) {
		return 1; //1表示当前正在服务器上运行
	}
	else {
		return 2; //2表示当前虚拟机已经释放
	}
}
/*end xuniji*/
/*begin fuuwqi*/
class FUWUQI
{
public:
	FUWUQI() {};
	int neihe;
	int neicun;
	int cost;
	int costOneDay;
	int neicunUsedA;
	int neicunUsedB;
	int neiheUsedA;
	int neiheUsedB;
	int id;
	int notUseDay; //闲置不用电的日期
	bool isUsed(); //是否使用
	string xinghao;
	FUWUQI(string &xinghao, int core, int memory, int cost, int costOneDay, int id);
	unordered_map<string, XUNIJI> id_xuniji_dict;
	//计算剩余内存
	int restNeicun() const;
	//计算A剩余内存
	int restNeicunA() const;
	//计算B剩余内存
	int restNeicunB() const;
	//计算剩余核数
	int restNeihe() const;
	// 计算A剩余核数
	int restNeiheA() const;
	//计算B剩余核数
	int restNeiheB() const;
	//计算剩余内存比核数
	double restNeicunNeiheRate() const;
	//计算A剩余内存比核数
	double restNeicunNeiheRateA() const;
	//计算B剩余内存比核数
	double restNeicunNeiheRateB() const;
	//判断是不是AB失衡了，返回0：A小，1：B小，2：均衡
	int shiHeng() const;
};
FUWUQI::FUWUQI(string &xinghao, int core, int memory, int cost, int costOneDay, int id) {
	this->notUseDay = -1;
	this->id = id;
	this->xinghao = xinghao;
	this->neihe = core;
	this->neicun = memory;
	this->cost = cost;
	this->costOneDay = costOneDay;
	this->neicunUsedA = 0;
	this->neicunUsedB = 0;
	this->neiheUsedA = 0;
	this->neiheUsedB = 0;
	this->id_xuniji_dict.reserve(500);
}
bool FUWUQI::isUsed() {
	if (this->neicunUsedA == 0 && this->neicunUsedB == 0 && this->neiheUsedA == 0 && this->neiheUsedB == 0) {
		return false;
	}
	else {
		return true;
	}
}
int FUWUQI::restNeicun() const {
	return neicun - neicunUsedA - neicunUsedB;
}
int FUWUQI::restNeicunA() const
{
	return neicun / 2 - neicunUsedA;
}
int FUWUQI::restNeicunB() const
{
	return neicun / 2 - neicunUsedB;
}
int FUWUQI::restNeihe() const
{
	return neihe - neiheUsedA - neiheUsedB;
}
int FUWUQI::restNeiheA() const
{
	return neihe / 2 - neiheUsedA;
}
int FUWUQI::restNeiheB() const
{
	return neihe / 2 - neiheUsedB;
}
double FUWUQI::restNeicunNeiheRate() const
{
	return (double)restNeicun() / (double)restNeihe();
}
double FUWUQI::restNeicunNeiheRateA() const
{
	return (double)restNeicunA() / (double)restNeiheA();
}
double FUWUQI::restNeicunNeiheRateB() const
{
	return (double)restNeicunB() / (double)restNeiheB();
}
int FUWUQI::shiHeng() const
{
	int rest_neicun_A = restNeicunA();
	int rest_neicun_B = restNeicunB();
	int rest_neihe_A = restNeiheA();
	int rest_neihe_B = restNeiheB();
	if (rest_neicun_A < rest_neicun_B && rest_neihe_A < rest_neihe_B) {
		return 0;
	}
	else if (rest_neicun_A > rest_neicun_B && rest_neihe_A > rest_neihe_B) {
		return 1;
	}
	else {
		return 2;
	}
}
/*end fuwuqi*/
/*GlobalMessage*/
class GlobalMessage
{
public:
	static GlobalMessage& Get();
	static void Set(string filepath);//读取数据的路径
	static void Set();//读取数据的路径
	static void CalXunijiWillContinueDay();//计算虚拟机会持续的时间
	//map<int,int> CalFuwuqiXingjiabi();//计算对于当前服务器的性价比 <服务器ID,分数>
	vector<vector<LogicalOperation> > op_list_vec;//操作序列集合
	unordered_map<string, XUNIJI> xinuji_map; //所有可选的虚拟机 <虚拟机的型号,虚拟机>
	unordered_map<string, XUNIJI> allneed_xinuji_map; //所有操作序列里面有的虚拟机<虚拟机id，虚拟机>
	vector<FUWUQI> fuwuqi_vec;//<服务器列表>
	int current_Day;//当前的天数
	int all_cost;//所有的花费
	int all_day;//所有天
	int fuwuqi_id;//服务器ID
	int max_core;
	int max_memory;
	int xuniji_count;//虚拟机数量
	int all_unadd_memory;//所有没有添加的虚拟机的内存  加权持续日期
	int all_unadd_core;//所有没有添加的虚拟机的核 加权持续日期
	int all_unadd_number;
private:
	GlobalMessage(); //默认构造
};
/*实现GlobalMessage*/
static shared_ptr<GlobalMessage> instance_;
string& trim(string& s)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
	}
	return s;
}
int splitString(const string& strSrc, const std::string& strDelims, vector<string>& strDest)
{
	typedef std::string::size_type ST;
	string delims = strDelims;
	std::string STR;
	if (delims.empty()) delims = "\n\r";
	ST pos = 0, LEN = strSrc.size();
	while (pos < LEN) {
		STR = "";
		while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN)) ++pos;
		if (pos == LEN) return strDest.size();
		while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN)) STR += strSrc[pos++];
		//std::cout << "[" << STR << "]";
		if (!STR.empty()) strDest.push_back(STR);
	}
	return strDest.size();
}
GlobalMessage::GlobalMessage() {
}

void GlobalMessage::Set(string filepath) {
	Get().fuwuqi_id = 0;
	ifstream infile;
	infile.open(filepath);
	if (!infile.is_open())
	{
		//cout << "找不到文件" << endl;
		return;
	}
	string temp;
	getline(infile, temp);
	int Wtype;
	int Vtype;
	int All_Day;
	Wtype = stoi(temp);
	for (int i = 0; i < Wtype; i++) {
		getline(infile, temp);
		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
		vector<string> strDest;
		splitString(temp, ",", strDest);
		string xinghao = strDest[0];
		int core = stoi(strDest[1]);
		int memory = stoi(strDest[2]);
		int cost = stoi(strDest[3]);
		int costOneDay = stoi(strDest[4]);
		FUWUQI f = FUWUQI(xinghao, core, memory, cost, costOneDay, -1);
		Get().fuwuqi_vec.push_back(f);
	}
	getline(infile, temp);
	Vtype = stoi(temp);
	for (int i = 0; i < Vtype; i++) {
		getline(infile, temp);
		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
		vector<string> strDest;
		splitString(temp, ",", strDest);
		string xinghao = strDest[0];
		int core = stoi(strDest[1]);
		int memory = stoi(strDest[2]);
		bool shuangjiedian = (bool)stoi(strDest[3]);
		XUNIJI x = XUNIJI(xinghao, core, memory, shuangjiedian);
		Get().xinuji_map[xinghao] = x;
	}
	getline(infile, temp);
	All_Day = stoi(temp);
	Get().all_day = All_Day;
	int countadd = 0;
	int countdel = 0;
	for (int i = 0; i < All_Day; i++) {
		getline(infile, temp);
		int All_Require = stoi(temp);
		vector<LogicalOperation> op_list;
		for (int j = 0; j < All_Require; j++) {
			getline(infile, temp);
			temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
			vector<string> strDest;
			splitString(temp, ",", strDest);
			if (strDest[0] == "add") {
				countadd++;
				LogicalOperation add_op(strDest[0], trim(strDest[1]), trim(strDest[2]));
				op_list.push_back(add_op);
			}
			else {
				countdel++;
				LogicalOperation del_op(strDest[0], trim(strDest[1]));
				op_list.push_back(del_op);
			}
		}
		Get().op_list_vec.push_back(op_list);
	}
	Get().current_Day = 0;
	Get().all_cost = 0;
	CalXunijiWillContinueDay();
	unordered_map<string, XUNIJI>& allneed_xinuji_map = Get().allneed_xinuji_map;
	unordered_map<string, XUNIJI>::iterator iter = allneed_xinuji_map.begin();
	Get().all_unadd_core = 0;
	Get().all_unadd_memory = 0;
	Get().max_core = 0;
	Get().max_memory = 0;
	Get().xuniji_count = 0;
	for (iter; iter != allneed_xinuji_map.end(); iter++) {
		Get().all_unadd_core += 1.0 * iter->second.neihe * iter->second.continue_day;
		Get().all_unadd_memory += 1.0 * iter->second.neicun * iter->second.continue_day;
		if (iter->second.shuangjiedian == true) {
			if (Get().max_core < iter->second.neihe) {
				Get().max_core = iter->second.neihe;
			}
			if (Get().max_memory < iter->second.neicun) {
				Get().max_memory = iter->second.neicun;
			}
		}
		else {
			if (Get().max_core < iter->second.neihe * 2) {
				Get().max_core = iter->second.neihe * 2;
			}
			if (Get().max_memory < iter->second.neicun * 2) {
				Get().max_memory = iter->second.neicun * 2;
			}
		}
	}
}


void GlobalMessage::Set() {
	int N;
	cin >> N;
	string te = "\n";
	getline(cin, te);
	for (int i = 0; i < N; i++) {
		char szBuf[256];
		cin.getline(szBuf, 256);
		string temp(szBuf);
		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
		vector<string> strDest;
		splitString(temp, ",", strDest);
		FUWUQI f = FUWUQI(strDest[0], stoi(strDest[1]), stoi(strDest[2]), stoi(strDest[3]), stoi(strDest[4]), -1);
		Get().fuwuqi_vec.push_back(f);
	}
	int M;
	cin >> M;
	getline(cin, te);
	for (int i = 0; i < M; i++) {
		char szBuf[256];
		cin.getline(szBuf, 256);
		string temp(szBuf);
		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
		vector<string> strDest;
		splitString(temp, ",", strDest);
		XUNIJI x = XUNIJI(strDest[0], stoi(strDest[1]), stoi(strDest[2]), (bool)stoi(strDest[3]));
		Get().xinuji_map[strDest[0]] = x;
	}
	int T;
	cin >> T;
	Get().all_day = T;
	getline(cin, te);
	for (int i = 0; i < T; i++) {
		int R;
		cin >> R;
		getline(cin, te);
		vector<LogicalOperation> op_list;
		for (int j = 0; j < R; j++) {
			char szBuf[256];
			cin.getline(szBuf, 256);
			string temp(szBuf);
			temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
			vector<string> strDest;
			splitString(temp, ",", strDest);
			if (strDest[0] == "add") {
				LogicalOperation add_op(strDest[0], trim(strDest[1]), trim(strDest[2]));
				op_list.push_back(add_op);
			}
			else {
				LogicalOperation del_op(strDest[0], trim(strDest[1]));
				op_list.push_back(del_op);
			}
		}
		Get().op_list_vec.push_back(op_list);
	}
	Get().current_Day = 0;
	Get().all_cost = 0;
	CalXunijiWillContinueDay();
	unordered_map<string, XUNIJI>& allneed_xinuji_map = Get().allneed_xinuji_map;
	unordered_map<string, XUNIJI>::iterator iter = allneed_xinuji_map.begin();
	Get().all_unadd_core = 0;
	Get().all_unadd_memory = 0;
	Get().max_core = 0;
	Get().max_memory = 0;
	Get().xuniji_count = 0;
	for (iter; iter != allneed_xinuji_map.end(); iter++) {
		Get().all_unadd_core += iter->second.neihe * iter->second.continue_day;
		Get().all_unadd_memory += iter->second.neicun * iter->second.continue_day;
		if (iter->second.shuangjiedian == true) {
			if (Get().max_core < iter->second.neihe) {
				Get().max_core = iter->second.neihe;
			}
			if (Get().max_memory < iter->second.neicun) {
				Get().max_memory = iter->second.neicun;
			}
		}
		else {
			if (Get().max_core < iter->second.neihe * 2) {
				Get().max_core = iter->second.neihe * 2;
			}
			if (Get().max_memory < iter->second.neicun * 2) {
				Get().max_memory = iter->second.neicun * 2;
			}
		}
	}
}
void GlobalMessage::CalXunijiWillContinueDay() {
	for (int i = 0; i < Get().op_list_vec.size(); i++) {
		for (auto& temp_op : Get().op_list_vec[i]) {
			int j = i;
			if (temp_op.op_name == "add") {
				Get().allneed_xinuji_map[temp_op.xuniji_id] = Get().xinuji_map[temp_op.xing_hao];
				Get().allneed_xinuji_map[temp_op.xuniji_id].start_day = j;
				Get().allneed_xinuji_map[temp_op.xuniji_id].continue_day = Get().all_day - Get().allneed_xinuji_map[temp_op.xuniji_id].start_day;
				Get().allneed_xinuji_map[temp_op.xuniji_id].end_day = Get().all_day + 1;
			}
			else if (temp_op.op_name == "del") {
				Get().allneed_xinuji_map[temp_op.xuniji_id].continue_day = j - Get().allneed_xinuji_map[temp_op.xuniji_id].start_day + 1;
				Get().allneed_xinuji_map[temp_op.xuniji_id].end_day = j + 1;
			}
		}
	}
}
//{
//	vector<vector<LogicalOperation> > op_list_vi = Get().op_list_vec;
//	unordered_map<string, XUNIJI> xinuji_map = Get().xinuji_map;
//	unordered_map<string, XUNIJI>& allneed_xinuji_map = Get().allneed_xinuji_map;
//	int all_day = op_list_vi.size();
//	for (int i = 0; i < all_day; i++) {
//		vector<LogicalOperation> op_list = op_list_vi[i];
//		for (int j = 0; j < op_list.size(); j++) {
//			LogicalOperation temp_op = op_list[j];
//			if (temp_op.op_name == "add") {
//				allneed_xinuji_map[temp_op.xuniji_id] = xinuji_map[temp_op.xing_hao];
//				allneed_xinuji_map[temp_op.xuniji_id].start_day = i;
//				allneed_xinuji_map[temp_op.xuniji_id].continue_day = 0;
//			}
//			else if (temp_op.op_name == "del") {
//				allneed_xinuji_map[temp_op.xuniji_id].continue_day = i - allneed_xinuji_map[temp_op.xuniji_id].start_day + 1;
//				allneed_xinuji_map[temp_op.xuniji_id].end_day = i + 1;
//			}
//		}
//	}
//	for (unordered_map<string, XUNIJI>::iterator iter = allneed_xinuji_map.begin(); iter != allneed_xinuji_map.end(); iter++) {
//		if (iter->second.continue_day == 0) {
//			iter->second.continue_day = all_day - iter->second.start_day;
//			iter->second.end_day = all_day + 1;
//		}
//	}
//}
GlobalMessage& GlobalMessage::Get() {
	if (instance_.get() == NULL) {
		instance_.reset(new GlobalMessage());
	}
	return *(instance_.get());
}
//end GlobalMessage
/*begin Buy*/
class Buy
{
public:
	double xingJiaBiAlgorithm(const FUWUQI& fuwuqi, double memory_core_rate, int end_day); //性价比计算方法
	FUWUQI buyOptimalMachine2(const XUNIJI &xuniji);//购买最佳机器 1.1版本
};
double Buy::xingJiaBiAlgorithm(const FUWUQI& fuwuqi, double memory_core_rate, int end_day) {
	//1.1版本，分子：看比例，是哪边失衡了，就按资源小的那个来算，例如，如果虚拟机内存核比2：1,
	//  现在有一个机器是90内存，核数5的，总天数10，当前天数6，购买价格10000，每日耗能500,性价比公式就是(5 + 0.2 * (90 / (2:1) - 5)) / (10000 + (10 - 6) * 500)
	//         其中0.2是chao_can1，是一个可调整的超参
	//先看比例是哪边大一些
	double fuwuqi_memory_core_rate = fuwuqi.restNeicunNeiheRate();
	int blance = 1.0;
	if (fuwuqi.neicun > GlobalMessage::Get().max_memory && fuwuqi.neihe > GlobalMessage::Get().max_core) {
		blance = 2.5;
	}
	if (fuwuqi_memory_core_rate > memory_core_rate) {
		return  blance * ((double)fuwuqi.neihe + BILI * (memory_core_rate / fuwuqi_memory_core_rate) * (((double)fuwuqi.neicun / memory_core_rate) - fuwuqi.neihe)) /
			(fuwuqi.cost + 1.0 * fuwuqi.costOneDay * ((double)end_day - GlobalMessage::Get().current_Day));
	}
	return blance * ((double)fuwuqi.neicun / memory_core_rate + BILI * (fuwuqi_memory_core_rate / memory_core_rate) * ((double)fuwuqi.neihe - (double)fuwuqi.neicun / memory_core_rate)) /
		(fuwuqi.cost + 1.0 * fuwuqi.costOneDay * ((double)end_day - GlobalMessage::Get().current_Day));
}
FUWUQI Buy::buyOptimalMachine2(const XUNIJI& xuniji) {
	//1.1策略：根据没有分配的虚拟机决定性价比参数;
	int current_day = GlobalMessage::Get().current_Day;
	double memory = (double)xuniji.neicun * xuniji.continue_day + GlobalMessage::Get().all_unadd_memory;
	double core = (double)xuniji.neihe * xuniji.continue_day + GlobalMessage::Get().all_unadd_core;
	double xuniji_memory_core_rate = 1.0 * memory / core;
	double max_xingjiabi = -1;//最大性价比
	FUWUQI res;
	for (FUWUQI& p : GlobalMessage::Get().fuwuqi_vec) {
		if (xuniji.shuangjiedian == true && (xuniji.neihe > p.neihe || xuniji.neicun > p.neicun)) continue;//无法满足要求
		if (xuniji.shuangjiedian == false && (xuniji.neihe > p.neihe / 2 || xuniji.neicun > p.neicun / 2)) continue;//无法满足要求
		double x = xingJiaBiAlgorithm(p, xuniji_memory_core_rate, xuniji.end_day);
		if (x > max_xingjiabi) {
			max_xingjiabi = x;
			res = p;
		}
	}
	return res;
}
/*end buy*/
/*printer*/
class WUFUQI_AB {
public:
	shared_ptr<FUWUQI> fuwuqi_ptr;
	int AB; // 1表示A 2表示B 3表示AB
	WUFUQI_AB() {};
	WUFUQI_AB(int ab, shared_ptr<FUWUQI> _fuwuqi_ptr) {
		fuwuqi_ptr = _fuwuqi_ptr;
		AB = ab;
	}
};

class Printer
{
public:
	Printer() {};
	void BuShu(WUFUQI_AB fuwuqi);
	void MigrationAdd(string id, WUFUQI_AB fuwuqi);
	void FuWuQiAdd(string fuwuqi_xinghao, shared_ptr<FUWUQI> fuwuqi_ptr);
	vector<WUFUQI_AB> bushu_vec;//当operate==bushu,一天部署的虚拟机<服务器id,结点>
	map<string, WUFUQI_AB> migration_map;//当operate==migration,一天迁移的虚拟机id和服务器id,结点
	map<string, vector<shared_ptr<FUWUQI>>> add_fuwuqi_map;//当operate=="purchase" 一天购买的服务器型号和对应的服务器指针
	void Print();
};
void Printer::BuShu(WUFUQI_AB fuwuqi) {
	this->bushu_vec.push_back(fuwuqi);
}
void Printer::MigrationAdd(string id, WUFUQI_AB fuwuqi) {
	this->migration_map[id] = fuwuqi;
}
void Printer::FuWuQiAdd(string fuwuqi_xinghao, shared_ptr<FUWUQI> fuwuqi_ptr) {
	this->add_fuwuqi_map[fuwuqi_xinghao].push_back(fuwuqi_ptr);
}

void Printer::Print() {
	//purchase migration bushu

	printf("(purchase,%d)\n", int(this->add_fuwuqi_map.size()));
	map<string, vector<shared_ptr<FUWUQI>>>::iterator iter = this->add_fuwuqi_map.begin();
	for (iter; iter != this->add_fuwuqi_map.end(); iter++) {
		printf("(%s,%d)\n", iter->first.c_str(), (int)iter->second.size());
		for (auto& ptr : iter->second) {
			ptr->id = GlobalMessage::Get().fuwuqi_id;
			GlobalMessage::Get().fuwuqi_id++;
		}
	}
	map<string, WUFUQI_AB>::iterator migration_iter = this->migration_map.begin();
	printf("(migration,%d)\n", (int)this->migration_map.size());
	for (migration_iter; migration_iter != this->migration_map.end(); migration_iter++) {
		if (migration_iter->second.AB == 3) {
			printf("(%s,%d)\n", migration_iter->first.c_str(), migration_iter->second.fuwuqi_ptr->id);

		}
		else if (migration_iter->second.AB == 1) {
			printf("(%s,%d,A)\n", migration_iter->first.c_str(), migration_iter->second.fuwuqi_ptr->id);

		}
		else if (migration_iter->second.AB == 2) {
			printf("(%s,%d,B)\n", migration_iter->first.c_str(), migration_iter->second.fuwuqi_ptr->id);

		}
	}
	for (int i = 0; i < this->bushu_vec.size(); i++) {
		if (this->bushu_vec[i].AB == 3) {
			printf("(%d)\n", this->bushu_vec[i].fuwuqi_ptr->id);
		}
		else if (this->bushu_vec[i].AB == 1) {
			printf("(%d,A)\n", this->bushu_vec[i].fuwuqi_ptr->id);
		}
		else if (this->bushu_vec[i].AB == 2) {
			printf("(%d,B)\n", this->bushu_vec[i].fuwuqi_ptr->id);
		}
	}
}
/*end printer*/
//begin MachineLibarary
//begin MachineLibarary


class MachineLibarary {
public:
	//区分A上部署还是B上部署还是双节点部署
	enum class AB {
		A = 1,
		B = 2,
		AB = 3,
		NO_AB = 4//未指定
	};
	virtual ~MachineLibarary() {
		if (m_instance_ptr != nullptr) {
			delete m_instance_ptr;
		}
	}
	static MachineLibarary* get_instance() {
		if (m_instance_ptr == nullptr) {
			m_instance_ptr = new MachineLibarary();
		}
		return m_instance_ptr;
	}
	unordered_map<std::shared_ptr<FUWUQI>, int> fuwuqi_ptr_id_dict;//服务器指针对应服务器ID
	pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > addXuNiJi(const string& id, XUNIJI xuniji, unordered_set<shared_ptr<FUWUQI>> &unused_set, unordered_set<shared_ptr<FUWUQI>> &in_it); //添加虚拟机,新增排除列表
	std::shared_ptr<FUWUQI> addFuWuQi(FUWUQI& fuwuqi);//添加一台新的机器
	void addXuNiJi(AB ab_flag, std::shared_ptr<FUWUQI> fuwuqi_ptr, const string& id, XUNIJI xuniji);//在指定的机器的指定节点上添加虚拟机
	void delXuNiJi(const string& id);//根据ID删除虚拟机
	pair<AB, shared_ptr<FUWUQI>> findOptimalFuWuQi_charu(XUNIJI& xuniji);//charu
	pair<AB, shared_ptr<FUWUQI>> findOptimalFuWuQi_qianyi(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>> &fuwuqi_ptr, unordered_set<shared_ptr<FUWUQI>> &in_it);//为一台虚拟机寻找最优的一个机器 1.2版本(滑窗法)
	int getCurrentCost();//计算每天的耗电花费
	int getfuwuqisize();
	vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> > > migaration(); //迁移
	unordered_map<string, pair< AB, std::shared_ptr<FUWUQI>>> id_fuwuqi_dict;//虚拟机id对应    机器指针and是A还是B节点部署的
private:
	list<std::shared_ptr<FUWUQI>> fuwuqi_list; //机器列表
	unordered_map<string, XUNIJI> id_xuniji_dict;//虚拟机id对应虚拟机信息
	MachineLibarary() {
		id_fuwuqi_dict.reserve(100);
		id_xuniji_dict.reserve(100);
		fuwuqi_ptr_id_dict.reserve(100);
	}
	MachineLibarary(MachineLibarary&) = delete;
	MachineLibarary& operator=(const MachineLibarary&) = delete;
	static MachineLibarary* m_instance_ptr;
};
//实现 MachineLibarary
MachineLibarary* MachineLibarary::m_instance_ptr = nullptr;
int MachineLibarary::getCurrentCost() {
	int cost = 0;
	for (std::shared_ptr<FUWUQI> p : fuwuqi_list) {
		if (p->isUsed()) {
			cost += p->costOneDay;
		}
	}
	return cost;
}
int MachineLibarary::getfuwuqisize()
{
	return fuwuqi_list.size();
}
bool cmp(std::shared_ptr<FUWUQI>& f1, std::shared_ptr<FUWUQI>& f2) {
	return f1->id_xuniji_dict.size() < f2->id_xuniji_dict.size();
}

//bool cmp(std::shared_ptr<FUWUQI>& f1, std::shared_ptr<FUWUQI>& f2) {
//	return  ((1.0 * ((double)f1->neiheUsedA + f1->neiheUsedB) / f1->neihe + 1.0 * ((double)f1->neicunUsedA + f1->neicunUsedB) / f1->neicun))< ((1.0 * ((double)f2->neiheUsedA + f2->neiheUsedB) / f2->neihe + 1.0 * ((double)f2->neicunUsedA + f2->neicunUsedB) / f2->neicun));
//}

//int max_count;
//int fengqucount;//迁移次数
//std::mutex k;//锁
//
//void run_qianyi(int beg,int end, vector<vector<shared_ptr<FUWUQI>>>& all_fuwuqi_fenqu, vector<unordered_set<shared_ptr<FUWUQI>>>& all_fuwuqi_fenqu_set) {
//	//double be = clock();
//	for (int i = beg; i < end; i++) {
//		if (all_fuwuqi_fenqu[i].size() <= 1) {
//			//cout << "小于1" << endl;
//			break;
//		}
//		unordered_set<shared_ptr<FUWUQI>> v;//已经迁移过的服务器
//		unordered_set<shared_ptr<FUWUQI>> v2;//不用迁移的服务器
//		mtx.lock();
//		while (fengqucount < max_count) {
//			mtx.unlock();
//			int min_size = 100000;
//			shared_ptr<FUWUQI> min_fuwuqi_ptr = nullptr;
//			for (auto& fuwuqi_ptr : all_fuwuqi_fenqu[i]) {
//				if (fuwuqi_ptr->id_xuniji_dict.size() == 0) continue;
//				if (v.find(fuwuqi_ptr) != v.end()) continue;
//				if (v2.find(fuwuqi_ptr) != v2.end()) continue;
//				if ((1.0 * ((double)fuwuqi_ptr->neiheUsedA + fuwuqi_ptr->neiheUsedB) / fuwuqi_ptr->neihe + 1.0 * ((double)fuwuqi_ptr->neicunUsedA + fuwuqi_ptr->neicunUsedB) / fuwuqi_ptr->neicun) > 1.8) {
//					v2.insert(fuwuqi_ptr);
//					continue;
//				}
//				//if (max_cnt - cnt < fuwuqi_ptr->id_xuniji_dict.size()) continue;
//				int distance = fuwuqi_ptr->id_xuniji_dict.size();
//				//double fuwuqi_core = fuwuqi_ptr->coreUsedA + fuwuqi_ptr->coreUsedB;
//				//double distance = fuwuqi_core * memory_core_rate + fuwuqi_memory;
//				if (distance < min_size) {
//					min_size = distance;
//					min_fuwuqi_ptr = fuwuqi_ptr;
//				}
//			}
//			//double afterfindfuwuqi = clock();
//			//findoptfuwuqi += (afterfindfuwuqi - beforefindfuwuqi) / 1000;
//			if (min_fuwuqi_ptr == NULL) {
//				break;
//			}
//			v.insert(min_fuwuqi_ptr);//不能迁移到自己身上
//			bool end_flag = false;
//			//将该机器上的虚拟机全部迁移出去
//			vector<string> need_migra_xuniji;
//			need_migra_xuniji.reserve(100);
//			//double beforecharuxuniji = clock();
//			for (auto& xuniji : min_fuwuqi_ptr->id_xuniji_dict) {
//				if (fengqucount >= max_count) {
//					end_flag = true;
//					break;
//				}
//				//先分配机器
//				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> old_p = MachineLibarary::get_instance()->id_fuwuqi_dict[xuniji.first];
//				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> p = MachineLibarary::get_instance()->addXuNiJi(xuniji.first, xuniji.second, v, all_fuwuqi_fenqu_set[i]);
//				if (p.second == nullptr) {
//					continue;
//				}
//				else {
//					//分配成功
//					qianyi_fenqu[i].push_back(make_pair(xuniji.first, p));
//					fengqucount++;
//					//移除原机器上的资源
//					need_migra_xuniji.push_back(xuniji.first);
//					if (old_p.first == MachineLibarary::AB::AB) {
//						//双节点部署
//						old_p.second->neiheUsedA -= xuniji.second.neihe / 2;
//						old_p.second->neicunUsedA -= xuniji.second.neicun / 2;
//						old_p.second->neiheUsedB -= xuniji.second.neihe / 2;
//						old_p.second->neicunUsedB -= xuniji.second.neicun / 2;
//					}
//					else if (old_p.first == MachineLibarary::AB::A) {
//						//在A上部署的
//						old_p.second->neiheUsedA -= xuniji.second.neihe;
//						old_p.second->neicunUsedA -= xuniji.second.neicun;
//					}
//					else if (old_p.first == MachineLibarary::AB::B) {
//						//在B上部署的
//						old_p.second->neiheUsedB -= xuniji.second.neihe;
//						old_p.second->neicunUsedB -= xuniji.second.neicun;
//					}
//				}
//			}
//			/*double aftercharuxuniji = clock();
//			charuxuniji += (aftercharuxuniji - beforecharuxuniji) / 1000;*/
//			//移除ID
//			for (int j = 0; j < need_migra_xuniji.size(); j++) {
//				min_fuwuqi_ptr->id_xuniji_dict.erase(need_migra_xuniji[j]);
//			}
//			if (min_fuwuqi_ptr->id_xuniji_dict.size() != 0) {
//				//确定最大时间
//				min_fuwuqi_ptr->notUseDay = 0;
//				for (auto& x : min_fuwuqi_ptr->id_xuniji_dict) {
//					if (x.second.end_day > min_fuwuqi_ptr->notUseDay) {
//						min_fuwuqi_ptr->notUseDay = x.second.end_day;
//					}
//				}
//			}
//			if (end_flag) {
//				break;
//			}
//		}
//	}
//	//double after = clock();
//	//cout << (after - be) / 1000 << endl;
//}

vector<vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>>> qianyi_fenqu;

void run_qianyi(int beg, int end, int fenqu_max_count, vector<vector<shared_ptr<FUWUQI>>>& all_fuwuqi_fenqu, vector<unordered_set<shared_ptr<FUWUQI>>>& all_fuwuqi_fenqu_set) {
	//double be = clock();
	for (int i = beg; i < end; i++) {
		if (all_fuwuqi_fenqu[i].size() <= 1) {
			//cout << "小于1" << endl;
			break;
		}
		unordered_set<shared_ptr<FUWUQI>> v;//已经迁移过的服务器
		unordered_set<shared_ptr<FUWUQI>> v2;//不用迁移的服务器
		int fengqucount = 0;
		while (fengqucount < fenqu_max_count) {
			double min_size = 100000;
			shared_ptr<FUWUQI> min_fuwuqi_ptr = nullptr;
			for (auto& fuwuqi_ptr : all_fuwuqi_fenqu[i]) {
				if (fuwuqi_ptr->id_xuniji_dict.size() == 0) continue;
				if (v.find(fuwuqi_ptr) != v.end()) continue;
				if (v2.find(fuwuqi_ptr) != v2.end()) continue;
				double temp = (1.0 * ((double)fuwuqi_ptr->neiheUsedA + fuwuqi_ptr->neiheUsedB) / fuwuqi_ptr->neihe + 1.0 * ((double)fuwuqi_ptr->neicunUsedA + fuwuqi_ptr->neicunUsedB) / fuwuqi_ptr->neicun);
				if (temp > 1.9) {
					v2.insert(fuwuqi_ptr);
					continue;
				}
				//if (max_cnt - cnt < fuwuqi_ptr->id_xuniji_dict.size()) continue;
				//int distance = fuwuqi_ptr->id_xuniji_dict.size();
				//double fuwuqi_core = fuwuqi_ptr->coreUsedA + fuwuqi_ptr->coreUsedB;
				//double distance = fuwuqi_core * memory_core_rate + fuwuqi_memory;
				if (temp < min_size) {
					min_size = temp;
					min_fuwuqi_ptr = fuwuqi_ptr;
				}
			}
			//double afterfindfuwuqi = clock();
			//findoptfuwuqi += (afterfindfuwuqi - beforefindfuwuqi) / 1000;
			if (min_fuwuqi_ptr == NULL) {
				break;
			}
			v.insert(min_fuwuqi_ptr);//不能迁移到自己身上
			bool end_flag = false;
			//将该机器上的虚拟机全部迁移出去
			vector<string> need_migra_xuniji;
			need_migra_xuniji.reserve(100);
			//double beforecharuxuniji = clock();
			for (auto& xuniji : min_fuwuqi_ptr->id_xuniji_dict) {
				if (fengqucount >= fenqu_max_count) {
					end_flag = true;
					break;
				}
				//先分配机器
				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> old_p = MachineLibarary::get_instance()->id_fuwuqi_dict[xuniji.first];
				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> p = MachineLibarary::get_instance()->addXuNiJi(xuniji.first, xuniji.second, v, all_fuwuqi_fenqu_set[i]);
				if (p.second == nullptr) {
					continue;
				}
				else {
					//分配成功
					qianyi_fenqu[i].push_back(make_pair(xuniji.first, p));
					fengqucount++;
					//移除原机器上的资源
					need_migra_xuniji.push_back(xuniji.first);
					if (old_p.first == MachineLibarary::AB::AB) {
						//双节点部署
						old_p.second->neiheUsedA -= xuniji.second.neihe / 2;
						old_p.second->neicunUsedA -= xuniji.second.neicun / 2;
						old_p.second->neiheUsedB -= xuniji.second.neihe / 2;
						old_p.second->neicunUsedB -= xuniji.second.neicun / 2;
					}
					else if (old_p.first == MachineLibarary::AB::A) {
						//在A上部署的
						old_p.second->neiheUsedA -= xuniji.second.neihe;
						old_p.second->neicunUsedA -= xuniji.second.neicun;
					}
					else if (old_p.first == MachineLibarary::AB::B) {
						//在B上部署的
						old_p.second->neiheUsedB -= xuniji.second.neihe;
						old_p.second->neicunUsedB -= xuniji.second.neicun;
					}
				}
			}
			/*double aftercharuxuniji = clock();
			charuxuniji += (aftercharuxuniji - beforecharuxuniji) / 1000;*/
			//移除ID
			for (int j = 0; j < need_migra_xuniji.size(); j++) {
				min_fuwuqi_ptr->id_xuniji_dict.erase(need_migra_xuniji[j]);
			}
			if (min_fuwuqi_ptr->id_xuniji_dict.size() != 0) {
				//确定最大时间
				min_fuwuqi_ptr->notUseDay = 0;
				for (auto& x : min_fuwuqi_ptr->id_xuniji_dict) {
					if (x.second.end_day > min_fuwuqi_ptr->notUseDay) {
						min_fuwuqi_ptr->notUseDay = x.second.end_day;
					}
				}
			}
			if (end_flag) {
				break;
			}
		}
	}
	//double after = clock();
	//cout << (after - be) / 1000 << endl;
}



vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> MachineLibarary::migaration() {


	int max_count = GlobalMessage::Get().xuniji_count * 5 / 1000;
	vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> res;//迁移集合
	res.reserve(100);
	int FENQU = 0;
	int fuwuqicount = 0;
	fuwuqicount = fuwuqi_list.size();
	FENQU = fuwuqicount / 750 + 1 ;
	qianyi_fenqu.reserve(FENQU);
	qianyi_fenqu.resize(FENQU);
	for (int i = 0; i < qianyi_fenqu.size(); i++) {
		qianyi_fenqu[i].clear();
	}
	vector<shared_ptr<FUWUQI>> all_fuwuqi_vec;//所有服务器
	vector<vector<shared_ptr<FUWUQI>>> all_fuwuqi_fenqu(FENQU);//所有分区
	vector<unordered_set<shared_ptr<FUWUQI>>> all_fuwuqi_fenqu_set(FENQU);
	for (auto& fuwuqi_ptr : fuwuqi_list) {
		if (fuwuqi_ptr->id_xuniji_dict.size() == 0) {
			continue;
		}
		all_fuwuqi_vec.push_back(fuwuqi_ptr);
	}
	sort(all_fuwuqi_vec.begin(), all_fuwuqi_vec.end(), cmp);
	for (int i = 0; i < all_fuwuqi_vec.size(); i++) {
		all_fuwuqi_fenqu[i % FENQU].push_back(all_fuwuqi_vec[i]);
		all_fuwuqi_fenqu_set[i % FENQU].insert(all_fuwuqi_vec[i]);
	}

	vector<std::thread> threads;
	//区内迁移
	int fenqu_max_count = max_count / FENQU;
	//double be = clock();
	if (FENQU <= 4) {
		for (int i = 0; i < FENQU; i++) {
			//run_qianyi(i, i + 1, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
			threads.push_back(std::thread(std::bind(run_qianyi, i, i+1, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set)));

		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			//run_qianyi(FENQU * i / 4, FENQU * (i + 1) / 4, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
			threads.push_back(std::thread(std::bind(run_qianyi, FENQU*i/4, FENQU * (i+1) / 4, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set)));
		}
	}

	for (auto& t : threads) {
		t.join();
	}
	/*double after = clock();
	cout << (after - be) / 1000 << endl;*/

	for (auto& p : qianyi_fenqu) {
		for (auto& q : p) {
			res.push_back(q);
		}
	}
	/*allcount += count;
	static double alltime = 0.0;
	cout << count<<"/"<< allcount << endl;*/
	/*cout << "找服务器" << findoptfuwuqi << endl;
	cout << "插虚拟机" << charuxuniji << endl;*/
	return res;
}
std::shared_ptr<FUWUQI> MachineLibarary::addFuWuQi(FUWUQI& fuwuqi) {
	fuwuqi.id = fuwuqi_list.size();
	fuwuqi_list.push_front(std::shared_ptr<FUWUQI>(new FUWUQI(fuwuqi)));
	return fuwuqi_list.front();
}
pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > MachineLibarary::addXuNiJi(const string& id, XUNIJI xuniji, unordered_set<shared_ptr<FUWUQI>> &unused_set, unordered_set<shared_ptr<FUWUQI>> &in_it) {
	//调用匹配算法匹配一个最佳机器
	pair< AB, std::shared_ptr<FUWUQI>> temp;
	if (unused_set.empty()) {
		temp = findOptimalFuWuQi_charu(xuniji);
	}
	else {
		temp = findOptimalFuWuQi_qianyi(xuniji, unused_set, in_it);
	}
	if (temp.second == nullptr) {
		//没有空余满足要求的机器了
		return temp;
	}
	addXuNiJi(temp.first, temp.second, id, xuniji);
	return temp;
}
void MachineLibarary::addXuNiJi(AB ab_flag, std::shared_ptr<FUWUQI> fuwuqi_ptr, const string& id, XUNIJI xuniji) {
	assert(fuwuqi_ptr != nullptr);
	//assert(id_fuwuqi_dict.find(id) == id_fuwuqi_dict.end());
	id_xuniji_dict[id] = xuniji; //添加绑定关系
	fuwuqi_ptr->id_xuniji_dict[id] = xuniji;
	if (ab_flag == AB::AB) {
		//TODO判断空间是否足够
		fuwuqi_ptr->neiheUsedA += (xuniji.neihe / 2);
		fuwuqi_ptr->neicunUsedA += (xuniji.neicun / 2);
		fuwuqi_ptr->neiheUsedB += (xuniji.neihe / 2);
		fuwuqi_ptr->neicunUsedB += (xuniji.neicun / 2);
		id_fuwuqi_dict[id] = make_pair(AB::AB, fuwuqi_ptr);
	}
	else if (ab_flag == AB::A) {
		//TODO判断空间是否足够
		fuwuqi_ptr->neiheUsedA += xuniji.neihe;
		fuwuqi_ptr->neicunUsedA += xuniji.neicun;
		id_fuwuqi_dict[id] = make_pair(AB::A, fuwuqi_ptr);
	}
	else if (ab_flag == AB::B) {
		//TODO判断空间是否足够
		fuwuqi_ptr->neiheUsedB += xuniji.neihe;
		fuwuqi_ptr->neicunUsedB += xuniji.neicun;
		id_fuwuqi_dict[id] = make_pair(AB::B, fuwuqi_ptr);
	}
	return;
}
void MachineLibarary::delXuNiJi(const string& id) {
	assert(id_fuwuqi_dict.find(id) != id_fuwuqi_dict.end());
	//找到对应机器
	pair<AB, std::shared_ptr<FUWUQI>> temp = id_fuwuqi_dict[id];
	shared_ptr<FUWUQI> fuwuqi_ptr = temp.second;
	AB ab_flag = temp.first;
	XUNIJI xuniji = id_xuniji_dict[id];
	//服务器减少对应虚拟机
	fuwuqi_ptr->id_xuniji_dict.erase(id);
	//减少该机器的相应节点中的核数和内存数
	if (ab_flag == AB::AB) {
		//双节点部署
		fuwuqi_ptr->neiheUsedA -= xuniji.neihe / 2;
		fuwuqi_ptr->neicunUsedA -= xuniji.neicun / 2;
		fuwuqi_ptr->neiheUsedB -= xuniji.neihe / 2;
		fuwuqi_ptr->neicunUsedB -= xuniji.neicun / 2;
	}
	else if (ab_flag == AB::A) {
		//在A上部署的
		fuwuqi_ptr->neiheUsedA -= xuniji.neihe;
		fuwuqi_ptr->neicunUsedA -= xuniji.neicun;
	}
	else if (ab_flag == AB::B) {
		//在B上部署的
		fuwuqi_ptr->neiheUsedB -= xuniji.neihe;
		fuwuqi_ptr->neicunUsedB -= xuniji.neicun;
	}
	//移除dict绑定关系
	id_fuwuqi_dict.erase(id);
	id_xuniji_dict.erase(id);
	return;
}
pair<MachineLibarary::AB, shared_ptr<FUWUQI>> MachineLibarary::findOptimalFuWuQi_charu(XUNIJI & xuniji)
{
	double memory = xuniji.neicun + GlobalMessage::Get().all_unadd_memory;
	double core = xuniji.neihe + GlobalMessage::Get().all_unadd_core;
	double xuniji_future_memory_core_rate_balance = 1.0 * memory / core;
	double xuniji_memory_core_rate = (double)xuniji.neicun / (double)xuniji.neihe;
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> best_choice;//选取的最优机器
	int currentDay = GlobalMessage::Get().current_Day; //当前日期
	//遍历所有机器，找比例最接近的节点
	if (xuniji.shuangjiedian == false) {
		//priority_queue < shared_ptr<FUWUQI>, vector<shared_ptr<FUWUQI>>,> min_dui;
		double diff_min_1 = 100000;
		double diff_min_2 = 100000;
		//单节点
		//unused_set.empty 表示插入 else 表示迁移
		list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
		for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
			if (!(*fuwuqi_ptr)->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			if ((*fuwuqi_ptr)->notUseDay <= currentDay) {
				continue;//已经不使用了
			}
			//先从服务器闲置日期>=虚拟机释放日期的服务器中分配
			if ((*fuwuqi_ptr)->notUseDay >= xuniji.end_day) {
				int shi_heng = (*fuwuqi_ptr)->shiHeng();//shi_heng表示是不是A比B多太多了？要保持AB的平衡
				double memory_core_rateA = (*fuwuqi_ptr)->restNeicunNeiheRateA();
				double memory_core_rateB = (*fuwuqi_ptr)->restNeicunNeiheRateB();
				//如果A空间不足，就只判断B，如果B空间不足，就只判断A
				if ((*fuwuqi_ptr)->restNeiheA() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunA() < xuniji.neicun) {
					shi_heng = 1;
				}
				if ((*fuwuqi_ptr)->restNeiheB() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunB() < xuniji.neicun) {
					shi_heng = 0;
				}
				if (shi_heng == 0) {
					//如果A使用太少了，就选取A
					//判断是否有剩余空间
					if ((*fuwuqi_ptr)->restNeiheA() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunA() < xuniji.neicun) {
						continue;
					}
					double temp = abs(memory_core_rateA - xuniji_memory_core_rate);
					double temp2 = (double)(*fuwuqi_ptr)->restNeiheA() / xuniji.neihe + (double)(*fuwuqi_ptr)->restNeicunA() / xuniji.neicun;
					if (temp < (diff_min_1 - MIN_DIFF)) {
						diff_min_1 = temp;
						diff_min_2 = temp2;
						best_choice = make_pair(AB::A, (*fuwuqi_ptr));
					}
					else if (temp < (diff_min_1 + MIN_DIFF)) {
						if (temp2 < diff_min_2) {
							diff_min_1 = temp;
							diff_min_2 = temp2;
							best_choice = make_pair(AB::A, (*fuwuqi_ptr));
						}
					}
				}
				else if (shi_heng == 1) {
					//选取B
					if ((*fuwuqi_ptr)->restNeiheB() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunB() < xuniji.neicun) {
						continue;
					}
					double temp = abs(memory_core_rateB - xuniji_memory_core_rate);
					double temp2 = (double)(*fuwuqi_ptr)->restNeiheB() / xuniji.neihe + (double)(*fuwuqi_ptr)->restNeicunB() / xuniji.neicun;
					if (temp < (diff_min_1 - MIN_DIFF)) {
						diff_min_1 = temp;
						diff_min_2 = temp2;
						best_choice = make_pair(AB::B, (*fuwuqi_ptr));
					}
					else if (temp < (diff_min_1 + MIN_DIFF)) {
						if (temp2 < diff_min_2) {
							diff_min_1 = temp;
							diff_min_2 = temp2;
							best_choice = make_pair(AB::B, (*fuwuqi_ptr));
						}
					}
				}
				else {
					//A和B都参与竞选
					double tempA = abs(memory_core_rateA - xuniji_memory_core_rate);
					double tempA2 = (double)(*fuwuqi_ptr)->restNeiheA() / xuniji.neihe + (double)(*fuwuqi_ptr)->restNeicunA() / xuniji.neicun;
					if (tempA < (diff_min_1 - MIN_DIFF)) {
						diff_min_1 = tempA;
						diff_min_2 = tempA2;
						best_choice = make_pair(AB::A, (*fuwuqi_ptr));
					}
					else if (tempA < (diff_min_1 + MIN_DIFF)) {
						if (tempA2 < diff_min_2) {
							diff_min_1 = tempA;
							diff_min_2 = tempA2;
							best_choice = make_pair(AB::A, (*fuwuqi_ptr));
						}
					}
					double tempB = abs(memory_core_rateB - xuniji_memory_core_rate);
					double tempB2 = (double)(*fuwuqi_ptr)->restNeiheB() / xuniji.neihe + (double)(*fuwuqi_ptr)->restNeicunB() / xuniji.neicun;
					if (tempB < (diff_min_1 - MIN_DIFF)) {
						diff_min_1 = tempB;
						diff_min_2 = tempB2;
						best_choice = make_pair(AB::B, (*fuwuqi_ptr));
					}
					else if (tempB < (diff_min_1 + MIN_DIFF)) {
						if (tempB2 < diff_min_2) {
							diff_min_1 = tempB;
							diff_min_2 = tempB2;
							best_choice = make_pair(AB::B, (*fuwuqi_ptr));
						}
					}
				}
			}
		}
		//如果没有找到
		if (best_choice.second == nullptr) {
			map<int, std::shared_ptr<FUWUQI>> fuwuqi_mp;


			list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
			for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {

				if (!(*fuwuqi_ptr)->isUsed()) {
					continue;//优先选正在使用的服务器
				}
				if ((*fuwuqi_ptr)->notUseDay <= currentDay) {
					continue;//已经不使用了
				}
				if ((*fuwuqi_ptr)->notUseDay < xuniji.end_day) {
					fuwuqi_mp[(*fuwuqi_ptr)->notUseDay] = *fuwuqi_ptr;
				}
			}
			map<int, std::shared_ptr<FUWUQI>>::reverse_iterator riter = fuwuqi_mp.rbegin();
			for (riter; riter != fuwuqi_mp.rend(); riter++) {
				int shi_heng = riter->second->shiHeng();
				if (riter->second->restNeiheA() < xuniji.neihe || riter->second->restNeicunA() < xuniji.neicun) {
					shi_heng = 1;
				}
				if (riter->second->restNeiheB() < xuniji.neihe || riter->second->restNeicunB() < xuniji.neicun) {
					shi_heng = 0;
				}
				if (shi_heng == 0) {
					//如果A使用太少了，就选取A
					//判断是否有剩余空间
					if (riter->second->restNeiheA() < xuniji.neihe || riter->second->restNeicunA() < xuniji.neicun) {
						continue;
					}
					riter->second->notUseDay = xuniji.end_day;
					best_choice = make_pair(AB::A, riter->second);
					break;
				}
				else if (shi_heng == 1) {
					//选取B
					if (riter->second->restNeiheB() < xuniji.neihe || riter->second->restNeicunB() < xuniji.neicun) {
						continue;
					}
					riter->second->notUseDay = xuniji.end_day;
					best_choice = make_pair(AB::B, riter->second);
					break;
				}
				else {
					riter->second->notUseDay = xuniji.end_day;
					if (riter->second->restNeiheA() > riter->second->restNeiheB()) {
						best_choice = make_pair(AB::A, riter->second);
					}
					else {
						best_choice = make_pair(AB::B, riter->second);
					}
					break;
				}
			}
		}
		//如果还没有找到,那就分配没有使用的服务器
		
		if (best_choice.second == nullptr) {
			double xingjiabi = -1;

			list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
			for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
				if ((*fuwuqi_ptr)->isUsed()) {
					continue;//选没有使用的服务器
				}
				if ((*fuwuqi_ptr)->restNeiheA() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunA() < xuniji.neicun) {
					continue;
				}
				if ((*fuwuqi_ptr)->restNeiheB() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunB() < xuniji.neicun) {
					continue;
				}
				if ((xuniji_future_memory_core_rate_balance * (*fuwuqi_ptr)->neihe + (*fuwuqi_ptr)->neicun) / (((double)xuniji.end_day - currentDay) * ((*fuwuqi_ptr)->costOneDay)) > xingjiabi) {
					xingjiabi = (xuniji_future_memory_core_rate_balance * (*fuwuqi_ptr)->neihe + (*fuwuqi_ptr)->neicun) / (((double)xuniji.end_day - currentDay) * ((*fuwuqi_ptr)->costOneDay));
					(*fuwuqi_ptr)->notUseDay = xuniji.end_day;
					best_choice = make_pair(AB::A, *fuwuqi_ptr);
				}
			}
		}
	}
	else {
		//双节点,策略是选AB加起来比例最接近的,且差不多的
		int target_memory = xuniji.neicun / 2;
		int target_core = xuniji.neihe / 2;
		double diff_min_1 = 100000.0; //比例距离
		double diff_min_2 = 100000.0;
		list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
		for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
			if (!(*fuwuqi_ptr)->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			if ((*fuwuqi_ptr)->notUseDay <= currentDay) {
				continue;//已经不使用了
			}
			//先淘汰内存不足的
			if ((*fuwuqi_ptr)->restNeiheA() < target_core || (*fuwuqi_ptr)->restNeiheB() < target_core ||
				(*fuwuqi_ptr)->restNeicunA() < target_memory || (*fuwuqi_ptr)->restNeicunB() < target_memory) {
				continue;
			}
			if ((*fuwuqi_ptr)->notUseDay >= xuniji.end_day) {
				double memory_core_rate = (*fuwuqi_ptr)->restNeicunNeiheRate();
				double temp = abs(memory_core_rate - xuniji_memory_core_rate);
				double temp2 = (double)(*fuwuqi_ptr)->restNeihe() / xuniji.neihe + (double)(*fuwuqi_ptr)->restNeicun() / xuniji.neicun;
				if (temp < (diff_min_1 - MIN_DIFF)) {
					diff_min_1 = temp;
					diff_min_2 = temp2;
					best_choice = make_pair(AB::AB, *fuwuqi_ptr);
				}
				else if (temp < (diff_min_1 + MIN_DIFF)) {
					if (((double)(*fuwuqi_ptr)->restNeihe() / xuniji.neihe + (double)(*fuwuqi_ptr)->restNeicun() / xuniji.neicun) < diff_min_2) {
						diff_min_1 = temp;
						diff_min_2 = temp2;
						best_choice = make_pair(AB::AB, *fuwuqi_ptr);
					}
				}
			}
		}
		
		if (best_choice.second == nullptr) {
			map<int, std::shared_ptr<FUWUQI>> fuwuqi_mp;
			list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
			for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
				if (!(*fuwuqi_ptr)->isUsed()) {
					continue;//优先选正在使用的服务器
				}
				if ((*fuwuqi_ptr)->notUseDay <= currentDay) {
					continue;//已经不使用了
				}
				if ((*fuwuqi_ptr)->notUseDay < xuniji.end_day) {
					fuwuqi_mp[(*fuwuqi_ptr)->notUseDay] = (*fuwuqi_ptr);
				}
			}
			map<int, std::shared_ptr<FUWUQI>>::reverse_iterator riter = fuwuqi_mp.rbegin();//从后面往前面插
			for (riter; riter != fuwuqi_mp.rend(); riter++) {
				//先淘汰内存不足的
				if (riter->second->restNeiheA() < target_core || riter->second->restNeiheB() < target_core ||
					riter->second->restNeicunA() < target_memory || riter->second->restNeicunB() < target_memory) {
					continue;
				}
				riter->second->notUseDay = xuniji.end_day;
				best_choice = make_pair(AB::AB, riter->second);
				break;
			}
		}
		//如果还没有找到,那就分配没有使用的服务器
		if (best_choice.second == nullptr) {
			double xingjiabi = -1;
			list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
			for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
				if ((*fuwuqi_ptr)->isUsed()) {
					continue;//选没有使用的服务器
				}
				if ((*fuwuqi_ptr)->restNeiheA() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunA() < xuniji.neicun) {
					continue;
				}
				if ((*fuwuqi_ptr)->restNeiheB() < xuniji.neihe || (*fuwuqi_ptr)->restNeicunB() < xuniji.neicun) {
					continue;
				}
				double temp = (xuniji_future_memory_core_rate_balance * (*fuwuqi_ptr)->neihe + (*fuwuqi_ptr)->neicun) / (xuniji.end_day - currentDay) * ((*fuwuqi_ptr)->costOneDay);
				if (temp > xingjiabi) {
					xingjiabi = temp;
					(*fuwuqi_ptr)->notUseDay = xuniji.end_day;
					best_choice = make_pair(AB::AB, *fuwuqi_ptr);
				}
			}
		}
	}
	return best_choice;
}

pair<MachineLibarary::AB, shared_ptr<FUWUQI>> MachineLibarary::findOptimalFuWuQi_qianyi(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& unused_set, unordered_set<shared_ptr<FUWUQI>>& in_it) {
	//1.2策略
	//double memory = xuniji.memory + GlobalMessage::Get().all_unadd_memory;
	//double core = xuniji.core + GlobalMessage::Get().all_unadd_core;
	//double xuniji_memory_core_rate_balance = 1.0 * memory / core;
	//版本1.2 滑窗法
	double memory = xuniji.neicun + GlobalMessage::Get().all_unadd_memory;
	double core = xuniji.neihe + GlobalMessage::Get().all_unadd_core;
	double xuniji_future_memory_core_rate_balance = 1.0 * memory / core;
	double xuniji_memory_core_rate = (double)xuniji.neicun / (double)xuniji.neihe;
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> optimal_machine;//选取的最优机器
	int currentDay = GlobalMessage::Get().current_Day; //当前日期
	//遍历所有机器，找比例最接近的节点
	if (xuniji.shuangjiedian == false) {
		//priority_queue < shared_ptr<FUWUQI>, vector<shared_ptr<FUWUQI>>,> min_dui;
		double min_distance = 100000;
		double min_distance2 = 100000;
		//单节点
		//unused_set.empty 表示插入 else 表示迁移
	
		for (auto& p : in_it) {
			if (unused_set.find(p) != unused_set.end()) continue;//如果机器已经迁移过，那就直接跳过
			if (!p->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			if (p->notUseDay <= currentDay) {
				continue;//已经不使用了
			}
			//先从服务器闲置日期>=虚拟机释放日期的服务器中分配
			if (p->notUseDay >= xuniji.end_day) {
				int shi_heng = p->shiHeng();//shi_heng表示是不是A比B多太多了？要保持AB的平衡
				double memory_core_rateA = p->restNeicunNeiheRateA();
				double memory_core_rateB = p->restNeicunNeiheRateB();
				//如果A空间不足，就只判断B，如果B空间不足，就只判断A
				if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
					shi_heng = 1;
				}
				if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
					shi_heng = 0;
				}
				if (shi_heng == 0) {
					//如果A使用太少了，就选取A
					//判断是否有剩余空间
					if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
						continue;
					}
					double temp = abs(memory_core_rateA - xuniji_memory_core_rate);
					double temp2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
					if (temp < (min_distance - MIN_DIFF)) {
						min_distance = temp;
						min_distance2 = temp2;
						optimal_machine = make_pair(AB::A, p);
					}
					else if (temp < (min_distance + MIN_DIFF)) {
						if (temp2 < min_distance2) {
							min_distance = temp;
							min_distance2 = temp2;
							optimal_machine = make_pair(AB::A, p);
						}
					}
				}
				else if (shi_heng == 1) {
					//选取B
					if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
						continue;
					}
					double temp = abs(memory_core_rateB - xuniji_memory_core_rate);
					double temp2 = (double)p->restNeiheB() / xuniji.neihe + (double)p->restNeicunB() / xuniji.neicun;
					if (temp < (min_distance - MIN_DIFF)) {
						min_distance = temp;
						min_distance2 = temp2;
						optimal_machine = make_pair(AB::B, p);
					}
					else if (temp < (min_distance + MIN_DIFF)) {
						if (temp2 < min_distance2) {
							min_distance = temp;
							min_distance2 = temp2;
							optimal_machine = make_pair(AB::B, p);
						}
					}
				}
				else {
					//A和B都参与竞选
					double tempA = abs(memory_core_rateA - xuniji_memory_core_rate);
					double tempA2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
					if (tempA < (min_distance - MIN_DIFF)) {
						min_distance = tempA;
						min_distance2 = tempA2;
						optimal_machine = make_pair(AB::A, p);
					}
					else if (tempA < (min_distance + MIN_DIFF)) {
						if (tempA2 < min_distance2) {
							min_distance = tempA;
							min_distance2 = tempA2;
							optimal_machine = make_pair(AB::A, p);
						}
					}
					double tempB = abs(memory_core_rateB - xuniji_memory_core_rate);
					double tempB2 = (double)p->restNeiheB() / xuniji.neihe + (double)p->restNeicunB() / xuniji.neicun;
					if (tempB < (min_distance - MIN_DIFF)) {
						min_distance = tempB;
						min_distance2 = tempB2;
						optimal_machine = make_pair(AB::B, p);
					}
					else if (tempB < (min_distance + MIN_DIFF)) {
						if (tempB2 < min_distance2) {
							min_distance = tempB;
							min_distance2 = tempB2;
							optimal_machine = make_pair(AB::B, p);
						}
					}
				}
			}
		}

		if (optimal_machine.second == nullptr) {
			map<int, std::shared_ptr<FUWUQI>> fuwuqi_mp;
			for (auto& p : in_it) {
				if (unused_set.find(p) != unused_set.end()) continue;//如果在不使用的虚拟机内就跳过
				if (!p->isUsed()) {
					continue;//优先选正在使用的服务器
				}
				if (p->notUseDay <= currentDay) {
					continue;//已经不使用了
				}
				if (p->notUseDay < xuniji.end_day) {
					fuwuqi_mp[p->notUseDay] = p;
				}
			}
			map<int, std::shared_ptr<FUWUQI>>::reverse_iterator riter = fuwuqi_mp.rbegin();
			for (riter; riter != fuwuqi_mp.rend(); riter++) {
				int shi_heng = riter->second->shiHeng();
				if (riter->second->restNeiheA() < xuniji.neihe || riter->second->restNeicunA() < xuniji.neicun) {
					shi_heng = 1;
				}
				if (riter->second->restNeiheB() < xuniji.neihe || riter->second->restNeicunB() < xuniji.neicun) {
					shi_heng = 0;
				}
				if (shi_heng == 0) {
					//如果A使用太少了，就选取A
					//判断是否有剩余空间
					if (riter->second->restNeiheA() < xuniji.neihe || riter->second->restNeicunA() < xuniji.neicun) {
						continue;
					}
					riter->second->notUseDay = xuniji.end_day;
					optimal_machine = make_pair(AB::A, riter->second);
					break;
				}
				else if (shi_heng == 1) {
					//选取B
					if (riter->second->restNeiheB() < xuniji.neihe || riter->second->restNeicunB() < xuniji.neicun) {
						continue;
					}
					riter->second->notUseDay = xuniji.end_day;
					optimal_machine = make_pair(AB::B, riter->second);
					break;
				}
				else {
					riter->second->notUseDay = xuniji.end_day;
					if (riter->second->restNeiheA() > riter->second->restNeiheB()) {
						optimal_machine = make_pair(AB::A, riter->second);
					}
					else {
						optimal_machine = make_pair(AB::B, riter->second);
					}
					break;
				}
			}
		}
	}
	else {
		//双节点,策略是选AB加起来比例最接近的,且差不多的
		int target_memory = xuniji.neicun / 2;
		int target_core = xuniji.neihe / 2;
		double min_distance = 100000.0; //比例距离
		double min_distance2 = 100000.0;
		
		for (auto& p : in_it) {
			if (unused_set.find(p) != unused_set.end()) continue;//如果机器已经迁移过，那就直接跳过
			if (!p->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			if (p->notUseDay <= currentDay) {
				continue;//已经不使用了
			}
			//先淘汰内存不足的
			if (p->restNeiheA() < target_core || p->restNeiheB() < target_core ||
				p->restNeicunA() < target_memory || p->restNeicunB() < target_memory) {
				continue;
			}
			if (p->notUseDay >= xuniji.end_day) {
				double memory_core_rate = p->restNeicunNeiheRate();
				double temp = abs(memory_core_rate - xuniji_memory_core_rate);
				double temp2 = (double)p->restNeihe() / xuniji.neihe + (double)p->restNeicun() / xuniji.neicun;
				if (temp < (min_distance - MIN_DIFF)) {
					min_distance = temp;
					min_distance2 = temp2;
					optimal_machine = make_pair(AB::AB, p);
				}
				else if (temp < (min_distance + MIN_DIFF)) {
					if (((double)p->restNeihe() / xuniji.neihe + (double)p->restNeicun() / xuniji.neicun) < min_distance2) {
						min_distance = temp;
						min_distance2 = temp2;
						optimal_machine = make_pair(AB::AB, p);
					}
				}
			}
		}
		if (optimal_machine.second == nullptr) {
			map<int, std::shared_ptr<FUWUQI>> fuwuqi_mp;
			for (auto& p : in_it) {
				if (unused_set.find(p) != unused_set.end()) continue;//如果在不使用的虚拟机内就跳过
				if (!p->isUsed()) {
					continue;//优先选正在使用的服务器
				}
				if (p->notUseDay <= currentDay) {
					continue;//已经不使用了
				}
				if (p->notUseDay < xuniji.end_day) {
					fuwuqi_mp[p->notUseDay] = p;
				}
			}
			map<int, std::shared_ptr<FUWUQI>>::reverse_iterator riter = fuwuqi_mp.rbegin();//从后面往前面插
			for (riter; riter != fuwuqi_mp.rend(); riter++) {
				//先淘汰内存不足的
				if (riter->second->restNeiheA() < target_core || riter->second->restNeiheB() < target_core ||
					riter->second->restNeicunA() < target_memory || riter->second->restNeicunB() < target_memory) {
					continue;
				}
				riter->second->notUseDay = xuniji.end_day;
				optimal_machine = make_pair(AB::AB, riter->second);
				break;
			}
		}
		
	}
	return optimal_machine;
}
//end MachineLibarary


int main()
{
	// TODO:read standard input
	GlobalMessage& temp = GlobalMessage::Get();
	temp.Set();
	//temp.Set("training-1.txt");
	Buy b;
	int cost = 0;
	//double be = clock();
	//从globalMessage里取任务执行
	for (int i = 0; i < GlobalMessage::Get().op_list_vec.size(); i++) {
		Printer printer;
		////迁移
		vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> >>> migration_vec = MachineLibarary::get_instance()->migaration();
		for (pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> >>& p : migration_vec) {
			printer.MigrationAdd(p.first, WUFUQI_AB(int(p.second.first), p.second.second));
		}
		int day = GlobalMessage::Get().op_list_vec[i].size();
		for (int j = 0; j < day; j++) {
			LogicalOperation& temp = GlobalMessage::Get().op_list_vec[i][j];
			if (temp.op_name == "add") {
				GlobalMessage::Get().xuniji_count++;
				//获取型号对应的虚拟机
				XUNIJI& xuniji = GlobalMessage::Get().allneed_xinuji_map[temp.xuniji_id];
				unordered_set<shared_ptr<FUWUQI>> unused_set;
				unordered_set<shared_ptr<FUWUQI>> in_it;
				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > p = MachineLibarary::get_instance()->addXuNiJi(temp.xuniji_id, xuniji, unused_set, in_it);
				if (p.second != nullptr) {
					//分配成功
					WUFUQI_AB wufuqi_ab((int)p.first, p.second);
					printer.BuShu(wufuqi_ab);
				}
				else {
					//购买机器
					FUWUQI new_fuwuqi = b.buyOptimalMachine2(xuniji);
					cost += new_fuwuqi.cost;
					new_fuwuqi.notUseDay = xuniji.end_day;
					//添加机器
					std::shared_ptr<FUWUQI> fuwuqi_ptr = MachineLibarary::get_instance()->addFuWuQi(new_fuwuqi);
					printer.FuWuQiAdd(new_fuwuqi.xinghao, fuwuqi_ptr);
					//添加虚拟机到机器
					if (xuniji.shuangjiedian == true) {
						MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::AB, fuwuqi_ptr, temp.xuniji_id, xuniji);
						p = make_pair(MachineLibarary::AB::AB, fuwuqi_ptr);
					}
					else {
						MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::A, fuwuqi_ptr, temp.xuniji_id, xuniji);
						p = make_pair(MachineLibarary::AB::A, fuwuqi_ptr);
					}
					WUFUQI_AB wufuqi_ab((int)p.first, p.second);
					printer.BuShu(wufuqi_ab);
				}
			}
			else if (temp.op_name == "del") {
				GlobalMessage::Get().xuniji_count--;
				MachineLibarary::get_instance()->delXuNiJi(temp.xuniji_id);
				GlobalMessage::Get().all_unadd_memory -= GlobalMessage::Get().allneed_xinuji_map[temp.xuniji_id].neicun * GlobalMessage::Get().allneed_xinuji_map[temp.xuniji_id].continue_day;
				GlobalMessage::Get().all_unadd_core -= GlobalMessage::Get().allneed_xinuji_map[temp.xuniji_id].neihe * GlobalMessage::Get().allneed_xinuji_map[temp.xuniji_id].continue_day;
			}
		}
		GlobalMessage::Get().current_Day++;
		printer.Print();
	}
	// TODO:process
	// TODO:write standard output
	// TODO:fflush(stdout);
	//double after = clock();
	//cout << (after - be) / 1000 << endl;
	return 0;
}
