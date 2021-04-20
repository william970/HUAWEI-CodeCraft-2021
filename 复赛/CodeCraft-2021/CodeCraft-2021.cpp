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
#include <mutex>
using namespace std;
#define MIN_DIFF 0.4
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
	XUNIJI(string& xinghao, int core, int memory, bool shuangjiedian);
	string xinghao;
	int neihe;
	int neicun;
	bool shuangjiedian;
	int getState(int current_day);//0 还未add，1 当前正在服务器上运行，2已经释放
	int start_day;//add的当前天
	//int continue_day; //将会持续的时间
	int end_day;//释放的日期
	int state;//当前状态，0 还未add，1 当前正在服务器上运行，2已经释放
	bool isSmall;//记录是否是小的虚拟机
	XUNIJI() = default;
};
XUNIJI::XUNIJI(string& xinghao, int core, int memory, bool shuangjiedian) {
	this->xinghao = xinghao;
	this->neihe = core;
	this->neicun = memory;
	this->shuangjiedian = shuangjiedian;
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
	//int notUseDay; //闲置不用电的日期
	
	string xinghao;
	FUWUQI(string& xinghao, int core, int memory, int cost, int costOneDay, int id);
	bool isUsed();//是否使用
	unordered_map<string, XUNIJI> id_xuniji_dict;

	//计算剩余核数
	int restNeihe() const;
	// 计算A剩余核数
	int restNeiheA() const;
	//计算B剩余核数
	int restNeiheB() const;

	//计算剩余内存
	int restNeicun() const;
	//计算A剩余内存
	int restNeicunA() const;
	//计算B剩余内存
	int restNeicunB() const; 

	//计算剩余内存比核数
	double restNeicunNeiheRate() const;
	//计算A剩余内存比核数
	double restNeicunNeiheRateA() const;
	//计算B剩余内存比核数
	double restNeicunNeiheRateB() const; 
	//判断是不是AB失衡了，返回0：A小，1：B小，2：均衡
	int shiHeng() const;
};
FUWUQI::FUWUQI(string& xinghao, int core, int memory, int cost, int costOneDay, int id) {
	//this->notUseDay = -1;
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

int FUWUQI::restNeicun() const
{
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

bool FUWUQI::isUsed() {
	if (this->id_xuniji_dict.size() == 0) {
		return false;
	}
	else {
		return true;
	}
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

/*end fuwuqi*/
/*GlobalMessage*/
class GlobalMessage
{
public:
	static GlobalMessage& Get();
	static void Set(string filepath);
	static void dealOneday();
	static void Init();
	//读取数据的路径
	static void Set();
	static void Step();
	//读取数据的路径
	static void CalXunijiWillContinueDay(int start, int end);//计算虚拟机会持续的时间
	//map<int,int> CalFuwuqiXingjiabi();//计算对于当前服务器的性价比 <服务器ID,分数>
	vector<vector<LogicalOperation> > op_list_vec;//操作序列集合
	unordered_map<string, XUNIJI> xinuji_map; //所有可选的虚拟机 <虚拟机的型号,虚拟机>
	unordered_map<string, XUNIJI> allneed_xinuji_map; //所有操作序列里面有的虚拟机<虚拟机id，虚拟机>
	unordered_map<int, int> add_op;
	unordered_map<int, int> del_op;
	vector<FUWUQI> fuwuqi_vec;//<服务器列表>
	int current_Day;//当前的天数
	int all_cost;//所有的花费
	int all_day;//所有天
	int forward_day;//向前大小
	int fuwuqi_id;//服务器ID
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
	double CalFuwuqiScore(const FUWUQI& fuwuqi, double memory_core_rate, double neihe_neicun_bili); //性价比计算方法
	FUWUQI buyOptimalMachine2(const XUNIJI& xuniji);//购买最佳机器 1.1版本
};
double Buy::CalFuwuqiScore(const FUWUQI& fuwuqi, double memory_core_rate, double neihe_neicun_bili) {
	//1.1版本，分子：看比例，是哪边失衡了，就按资源小的那个来算，例如，如果虚拟机内存核比2：1,
	//  现在有一个机器是90内存，核数5的，总天数10，当前天数6，购买价格10000，每日耗能500,性价比公式就是(5 + 0.2 * (90 / (2:1) - 5)) / (10000 + (10 - 6) * 500)
	//         其中0.2是chao_can1，是一个可调整的超参
	//先看比例是哪边大一些
	double fuwuqi_memory_core_rate = fuwuqi.restNeicunNeiheRate();
	double blance = 1.0;
	double bili = 0.0;
	//if (fuwuqi.neicun > GlobalMessage::Get().max_memory && fuwuqi.neihe > GlobalMessage::Get().max_core) 
	//{
	//	blance += 0.25;
	//}
	if (fuwuqi_memory_core_rate > memory_core_rate) {
		if (neihe_neicun_bili < 1) {
			blance += 100;
		}
		return  blance * (neihe_neicun_bili * (double)fuwuqi.neihe + bili * (memory_core_rate / fuwuqi_memory_core_rate) * (((double)fuwuqi.neicun / memory_core_rate) - fuwuqi.neihe)) /
			(fuwuqi.cost + 1.0 * fuwuqi.costOneDay * ((double)GlobalMessage::Get().all_day - GlobalMessage::Get().current_Day));
	}
	else {
		if (neihe_neicun_bili > 1) {
			blance += 100;
		}
		return blance * ((double)fuwuqi.neicun / (neihe_neicun_bili * memory_core_rate) + bili * (fuwuqi_memory_core_rate / memory_core_rate) * ((double)fuwuqi.neihe - (double)fuwuqi.neicun / memory_core_rate)) /
			(fuwuqi.cost + 1.0 * fuwuqi.costOneDay * ((double)GlobalMessage::Get().all_day - GlobalMessage::Get().current_Day));
	}
}

/*end buy*/

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
	//添加虚拟机,新增排除列表
	inline pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > addXuNiJi(const string& id, XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& unused_set, unordered_set<shared_ptr<FUWUQI>>& in_it, bool not_need_mig = true) {
		pair< AB, std::shared_ptr<FUWUQI>> temp;
		if (unused_set.empty() && in_it.empty() && not_need_mig) {
			temp = findOptimalFuWuQi_charu(xuniji);
		}
		//else if (!not_need_mig) {
		//	temp = findOptimalFuWuQi_huachuang_qianyi(xuniji, in_it);
		//}
		else {
			temp = findOptimalFuWuQi_fenqu_qianyi(xuniji, unused_set, in_it);
			//if (in_it.begin()->get()->notUseDay == GlobalMessage::Get().all_day) {
			//	
			//}
			//else {
			//	temp = findOptimalFuWuQi_fenqu_qianyi_2(xuniji, unused_set, in_it);
			//}
		}
		if (temp.second == nullptr) {
			//没有空余满足要求的机器了
			return temp;
		}
		addXuNiJi(temp.first, temp.second, id, xuniji);
		return temp;
	}
	std::shared_ptr<FUWUQI> addFuWuQi(FUWUQI& fuwuqi);//添加一台新的机器
	void addXuNiJi(AB ab_flag, std::shared_ptr<FUWUQI> fuwuqi_ptr, const string& id, XUNIJI& xuniji);//在指定的机器的指定节点上添加虚拟机
	void delXuNiJi(const string& id);//根据ID删除虚拟机
	pair<AB, shared_ptr<FUWUQI>> findOptimalFuWuQi_charu(XUNIJI& xuniji);
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> findOptimalFuWuQi_huachuang_qianyi(XUNIJI& xuniji,unordered_set<shared_ptr<FUWUQI>>& in_it);
	//charu
	pair<AB, shared_ptr<FUWUQI>> findOptimalFuWuQi_fenqu_qianyi(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& fuwuqi_ptr, unordered_set<shared_ptr<FUWUQI>>& in_it);
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> findOptimalFuWuQi_fenqu_qianyi_2(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& unused_set, unordered_set<shared_ptr<FUWUQI>>& in_it);
	int getALL();
	int getUnUsed();
	inline double getNeiheliyonglv() {
		int all_neihe = 0;
		int use_neihe = 0;
		for (std::shared_ptr<FUWUQI>& p : fuwuqi_list) {
			if (p->isUsed()) {
				all_neihe += p->neihe;
				use_neihe += p->neiheUsedA + p->neiheUsedB;
			}
		}
		if (all_neihe == 0) {
			return 1.0;
		}
		return 1.0 * use_neihe / all_neihe;
	}
	inline double getNeicunliyonglv() {
		int all_neicun = 0;
		int use_neicun = 0;
		for (std::shared_ptr<FUWUQI>& p : fuwuqi_list) {
			if (p->isUsed()) {
				all_neicun += p->neicun;
				use_neicun += p->neicunUsedA + p->neicunUsedB;
			}
		}
		if (all_neicun == 0) {
			return 1.0;
		}
		return 1.0 * use_neicun / all_neicun;
	}
	//为一台虚拟机寻找最优的一个机器 1.2版本(滑窗法)
	int getCurrentCost();//计算每天的耗电花费
	int getfuwuqisize();
	vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> > > migaration(); //迁移
	unordered_map<string, pair< AB, std::shared_ptr<FUWUQI>>> id_fuwuqi_dict;//虚拟机id对应    机器指针and是A还是B节点部署的
	unordered_map<string, XUNIJI> id_xuniji_dict;//虚拟机id对应虚拟机信息
private:
	list<std::shared_ptr<FUWUQI>> fuwuqi_list; //机器列表

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

int MachineLibarary::getALL()
{
	return fuwuqi_list.size();
}

int MachineLibarary::getUnUsed()
{
	int count = 0;
	for (auto& p : fuwuqi_list) {
		if (!p->isUsed()) {
			count++;
		}
	}

	return count;
}



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

vector<vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>>> qianyi_fenqu;

int qianyi_all_count;
int qianyi_count;
double not_mig;
std::mutex mtx;
void run_qianyi(int beg, int end, vector<vector<shared_ptr<FUWUQI>>>& all_fuwuqi_fenqu, vector<unordered_set<shared_ptr<FUWUQI>>>& all_fuwuqi_fenqu_set) {
	//double be = clock();
	for (int i = beg; i < end; i++) {
		if (all_fuwuqi_fenqu[i].size() <= 1) {
			//cout << "小于1" << endl;
			break;
		}
		unordered_set<shared_ptr<FUWUQI>> v;//已经迁移过的服务器
		unordered_set<shared_ptr<FUWUQI>> v2;//不用迁移的服务器
		while (qianyi_count < qianyi_all_count) {
			mtx.lock();
			if (qianyi_count >= qianyi_all_count) {
				mtx.unlock();
				break;
			}
			else {
				mtx.unlock();
			}
			int min_size = 100000;
			shared_ptr<FUWUQI> min_fuwuqi_ptr = nullptr;
			for (auto& fuwuqi_ptr : all_fuwuqi_fenqu[i]) {
				if (fuwuqi_ptr->id_xuniji_dict.size() == 0) continue;
				if (v.find(fuwuqi_ptr) != v.end()) continue;
				if (v2.find(fuwuqi_ptr) != v2.end()) continue;
				double temp = ((double)fuwuqi_ptr->neiheUsedA + fuwuqi_ptr->neiheUsedB) / fuwuqi_ptr->neihe + ((double)fuwuqi_ptr->neicunUsedA + fuwuqi_ptr->neicunUsedB) / fuwuqi_ptr->neicun;
				if (temp > not_mig) {
					v2.insert(fuwuqi_ptr);
					continue;
				}
				//if (max_cnt - cnt < fuwuqi_ptr->id_xuniji_dict.size()) continue;
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
				mtx.lock();
				if (qianyi_count >= qianyi_all_count) {
					mtx.unlock();
					end_flag = true;
					break;
				}
				else {
					qianyi_count++;
					mtx.unlock();
				}
				//先分配机器
				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> old_p = MachineLibarary::get_instance()->id_fuwuqi_dict[xuniji.first];
				pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> p = MachineLibarary::get_instance()->addXuNiJi(xuniji.first, xuniji.second, v, all_fuwuqi_fenqu_set[i]);
				if (p.second == nullptr) {
					mtx.lock();
					qianyi_count--;
					mtx.unlock();
					continue;

				}
				else {
					//分配成功
					mtx.lock();
					qianyi_fenqu[i].push_back(make_pair(xuniji.first, p));

					mtx.unlock();
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
			//if (min_fuwuqi_ptr->id_xuniji_dict.size() != 0) {
			//	//确定最大时间
			//	min_fuwuqi_ptr->notUseDay = 0;
			//	for (auto& x : min_fuwuqi_ptr->id_xuniji_dict) {
			//		if (x.second.end_day > min_fuwuqi_ptr->notUseDay) {
			//			min_fuwuqi_ptr->notUseDay = x.second.end_day;
			//		}
			//	}
			//}
			if (end_flag) {
				break;
			}
		}

	}
	//double after = clock();
	//cout << (after - be) / 1000 << endl;
}




vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> MachineLibarary::migaration() {

	//unordered_set<shared_ptr<FUWUQI>> temp_fuwuqi;
	//temp_fuwuqi.reserve(500);
	//for (auto& empty_fuwuqi: fuwuqi_list) {
	//	if (!empty_fuwuqi->isUsed()) {
	//		temp_fuwuqi.insert(empty_fuwuqi);
	//	}
	//}
	//vector<string> mig_xuniji;
	//mig_xuniji.reserve(500);
	//for (auto& xuniji : id_xuniji_dict) {
	//	if (xuniji.second.end_day == GlobalMessage::Get().current_Day) {
	//		mig_xuniji.push_back(xuniji.first);
	//	}
	//}
	////cout << "add:" << GlobalMessage::Get().add_op[GlobalMessage::Get().current_Day] << endl;
	////cout << mig_xuniji.size() << ":" << id_xuniji_dict.size() << endl;
	int count = 0;//迁移次数
	int max_count = GlobalMessage::Get().xuniji_count * 3 / 100;
	int min_count = max_count * 1 / 2;

	vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> res;//迁移集合
	res.reserve(100);

	//for (string& xuniji_id_string : mig_xuniji) {
	//	if (max_count <= 0) {
	//		break;
	//	}
	//	pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> old_p = MachineLibarary::get_instance()->id_fuwuqi_dict[xuniji_id_string];
	//	XUNIJI& need_mig = MachineLibarary::get_instance()->id_xuniji_dict[xuniji_id_string];
	//	unordered_set<shared_ptr<FUWUQI>> unused_set;
	//	unordered_set<shared_ptr<FUWUQI>> in_it;
	//	pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>> p = MachineLibarary::get_instance()->addXuNiJi(xuniji_id_string, need_mig, unused_set, temp_fuwuqi, false);
	//	if (p.second == nullptr) {
	//		continue;
	//	}
	//	else {
	//		max_count--;
	//		res.push_back(make_pair(xuniji_id_string, p));
	//		//移除原机器上的资源
	//		old_p.second->id_xuniji_dict.erase(xuniji_id_string);
	//		if (old_p.first == MachineLibarary::AB::AB) {
	//			//双节点部署
	//			old_p.second->neiheUsedA -= need_mig.neihe / 2;
	//			old_p.second->neicunUsedA -= need_mig.neicun / 2;
	//			old_p.second->neiheUsedB -= need_mig.neihe / 2;
	//			old_p.second->neicunUsedB -= need_mig.neicun / 2;
	//		}
	//		else if (old_p.first == MachineLibarary::AB::A) {
	//			//在A上部署的
	//			old_p.second->neiheUsedA -= need_mig.neihe;
	//			old_p.second->neicunUsedA -= need_mig.neicun;
	//		}
	//		else if (old_p.first == MachineLibarary::AB::B) {
	//			//在B上部署的
	//			old_p.second->neiheUsedB -= need_mig.neihe;
	//			old_p.second->neicunUsedB -= need_mig.neicun;
	//		}
	//	}
	//}


	double neiheliyonglv = MachineLibarary::get_instance()->getNeiheliyonglv();
	double neicunliyonglv = MachineLibarary::get_instance()->getNeicunliyonglv();
	
	not_mig = 1.9;

	int FENQU = 0;
	int fuwuqicount = 0;
	fuwuqicount = fuwuqi_list.size();
	FENQU = (fuwuqicount / 1000 + 1);
	if (neiheliyonglv + neicunliyonglv < 1.9) {
		FENQU = 1;
	}
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

	int fenquindex = 0;
	int mig_index = 0;
	for (int i = 0; i < all_fuwuqi_vec.size(); i++) {
		if (!all_fuwuqi_vec[i]->isUsed()) {
			continue;
		}
		fenquindex++;
		all_fuwuqi_fenqu[fenquindex % (FENQU)].push_back(all_fuwuqi_vec[i]);
		all_fuwuqi_fenqu_set[fenquindex % (FENQU)].insert(all_fuwuqi_vec[i]);
	}

	vector<std::thread> threads;
	//区内迁移
	qianyi_all_count = max_count;
	qianyi_count = 0;
	//run_qianyi(0, FENQU, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
	double be = clock();
	if (FENQU <= 4) {
		for (int i = 0; i < FENQU; i++) {
			//run_qianyi(i, i + 1, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
			threads.push_back(std::thread(std::bind(run_qianyi, i, i + 1, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set)));

		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			//run_qianyi(FENQU * i / 4, FENQU * (i + 1) / 4, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
			threads.push_back(std::thread(std::bind(run_qianyi, FENQU * i / 4, FENQU * (i + 1) / 4, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set)));
		}
	}
	//cout << "unused:" << MachineLibarary::get_instance()->getUnUsed() << "/" << MachineLibarary::get_instance()->getALL() << endl;
	for (auto& t : threads) {
		t.join();
	}
	//分区迁移

	/*double after = clock();
	cout << (after - be) / 1000 << endl;*/
	//cout << "unused:" << MachineLibarary::get_instance()->getUnUsed() << "/" << MachineLibarary::get_instance()->getALL() << endl;
	//cout << "all:" << qianyi_all_count << "/" << qianyi_count << endl;
	for (auto& p : qianyi_fenqu) {
		count += p.size();
		for (auto& q : p) {
			res.push_back(q);
		}
	}


	//qianyi_fenqu.clear();
	//for (int i = 0; i < FENQU; i++) {
	//	all_fuwuqi_fenqu[i].clear();
	//	all_fuwuqi_fenqu_set[i].clear();
	//}

	//threads.clear();
	//for (auto& fuwuqi_ptr : fuwuqi_list) {
	//	if (fuwuqi_ptr->id_xuniji_dict.size() == 0) {
	//		continue;
	//	}
	//	all_fuwuqi_vec.push_back(fuwuqi_ptr);
	//}
	//sort(all_fuwuqi_vec.begin(), all_fuwuqi_vec.end(), cmp);

	//fenquindex = 0;
	//mig_index = 0;
	//for (int i = 0; i < all_fuwuqi_vec.size(); i++) {
	//	if (!all_fuwuqi_vec[i]->isUsed()) {
	//		continue;
	//	}
	//	if (all_fuwuqi_vec[i]->notUseDay != GlobalMessage::Get().all_day) {
	//		mig_index++;
	//		all_fuwuqi_fenqu[FENQU - 1].push_back(all_fuwuqi_vec[i]);
	//		all_fuwuqi_fenqu_set[FENQU - 1].insert(all_fuwuqi_vec[i]);
	//		continue;
	//	}
	//	fenquindex++;
	//	all_fuwuqi_fenqu[fenquindex % (FENQU - 1)].push_back(all_fuwuqi_vec[i]);
	//	all_fuwuqi_fenqu_set[fenquindex % (FENQU - 1)].insert(all_fuwuqi_vec[i]);
	//}
	//max_count -= qianyi_count;
	////cout << fenquindex << " : vs : " << mig_index << endl;
	////区内迁移
	//fenqu_max_count = max_count / FENQU;
	////double be = clock();
	//if (FENQU <= 4) {
	//	for (int i = 0; i < FENQU; i++) {
	//		//run_qianyi(i, i + 1, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
	//		threads.push_back(std::thread(std::bind(run_qianyi, i, i + 1, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set)));

	//	}
	//}
	//else {
	//	for (int i = 0; i < 4; i++) {
	//		//run_qianyi(FENQU * i / 4, FENQU * (i + 1) / 4, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set);
	//		threads.push_back(std::thread(std::bind(run_qianyi, FENQU * i / 4, FENQU * (i + 1) / 4, fenqu_max_count, all_fuwuqi_fenqu, all_fuwuqi_fenqu_set)));
	//	}
	//}

	//for (auto& t : threads) {
	//	t.join();
	//}
	///*double after = clock();
	//cout << (after - be) / 1000 << endl;*/

	//for (auto& p : qianyi_fenqu) {
	//	count += p.size();
	//	for (auto& q : p) {
	//		res.push_back(q);
	//	}
	//}
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


void MachineLibarary::addXuNiJi(AB ab_flag, std::shared_ptr<FUWUQI> fuwuqi_ptr, const string& id, XUNIJI& xuniji) {
	assert(fuwuqi_ptr != nullptr);
	//assert(id_fuwuqi_dict.find(id) == id_fuwuqi_dict.end());
	id_xuniji_dict[id] = xuniji; //添加绑定关系
	fuwuqi_ptr->id_xuniji_dict[id] = xuniji;
	if (ab_flag == AB::AB) {
		//TODO判断空间是否足够
		fuwuqi_ptr->neiheUsedB += (xuniji.neihe / 2);
		fuwuqi_ptr->neicunUsedB += (xuniji.neicun / 2);
		fuwuqi_ptr->neiheUsedA += (xuniji.neihe / 2);
		fuwuqi_ptr->neicunUsedA += (xuniji.neicun / 2);

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

pair<MachineLibarary::AB, shared_ptr<FUWUQI>> MachineLibarary::findOptimalFuWuQi_charu(XUNIJI& xuniji)
{

	/*
	分两级
		第二级 表示是单节点还是双节点
		第三级 表示当前阶段
	*/
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> optimal_machine;//最优机器
	
	double xuniji_memory_core_rate = (double)xuniji.neicun / (double)xuniji.neihe;
	double core = 1.0*xuniji.neihe + GlobalMessage::Get().all_unadd_core;
	double memory = 1.0 * xuniji.neicun + GlobalMessage::Get().all_unadd_memory;
	double xuniji_future_memory_core_rate_balance = 1.0 * memory / core;
	
	int currentDay = GlobalMessage::Get().current_Day; //当前日期
	//遍历所有机器，找比例最接近的节点
	//如果不知道该虚拟机的释放日期的话

	if (xuniji.shuangjiedian == false) {
		//priority_queue < shared_ptr<FUWUQI>, vector<shared_ptr<FUWUQI>>,> min_dui;
		double diff_min1 = 100000;
		double diff_min2 = 100000;
		//单节点
		//1.1.1
		for (auto& p : fuwuqi_list) {
			//if (p->notUseDay != GlobalMessage::Get().all_day) {
			//	continue;
			//}
			if (!p->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			int shi_heng = p->shiHeng();//shi_heng表示是不是A比B多太多了？要保持AB的平衡
			//如果A空间不足，就只判断B，如果B空间不足，就只判断A
			if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
				shi_heng = 1;
			}
			if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
				shi_heng = 0;
			}

			double memory_core_rateA = p->restNeicunNeiheRateA();
			double memory_core_rateB = p->restNeicunNeiheRateB();
			if (shi_heng == 0) {
				//如果A使用太少了，就选取A
				//判断是否有剩余空间
				if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
					continue;
				}
				double temp = abs(memory_core_rateA - xuniji_memory_core_rate);
				double temp2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
				if (temp < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::A, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = temp;
						diff_min2 = temp2;
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
				if (temp < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::B, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						optimal_machine = make_pair(AB::B, p);
						diff_min1 = temp;
						diff_min2 = temp2;
					}
				}
			}
			else {

				//A和B都参与竞选
				double tempA = abs(memory_core_rateA - xuniji_memory_core_rate);
				double tempA2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
				if (tempA < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::A, p);
					diff_min1 = tempA;
					diff_min2 = tempA2;
				}
				else if (tempA < (diff_min1 + MIN_DIFF)) {
					if (tempA2 < diff_min2) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = tempA;
						diff_min2 = tempA2;
					}
				}
				double tempB = abs(memory_core_rateB - xuniji_memory_core_rate);
				double tempB2 = (double)p->restNeicunA() / xuniji.neihe + (double)p->restNeicunB() / xuniji.neicun;
				if (tempB < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::B, p);
					diff_min1 = tempB;
					diff_min2 = tempB2;
				}
				else if (tempB < (diff_min1 + MIN_DIFF)) {
					if (tempB2 < diff_min2) {
						optimal_machine = make_pair(AB::B, p);
						diff_min1 = tempB;
						diff_min2 = tempB2;
					}
				}
			}
		}

		//如果还没有找到,那就分配没有使用的服务器1.1.2
		if (optimal_machine.second == nullptr) {
			//double xingjiabi = -1;
			double diff_min1 = 100000;
			double diff_min2 = 100000;
			for (auto& p : fuwuqi_list) {
				if (p->isUsed()) {
					continue;//选没有使用的服务器
				}
				if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
					continue;
				}
				if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
					continue;
				}
				//if ((xuniji_future_memory_core_rate_balance * p->neihe + p->neicun) / (((double)xuniji.end_day - currentDay) * (p->costOneDay)) > xingjiabi) {
				//	xingjiabi = (xuniji_future_memory_core_rate_balance * p->neihe + p->neicun) / (((double)xuniji.end_day - currentDay) * (p->costOneDay));
				//	optimal_machine = make_pair(AB::A, p);
				//}
				double memory_core_rateA = p->restNeicunNeiheRateA();
				if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
					continue;
				}
				double temp = abs(memory_core_rateA - xuniji_memory_core_rate);
				double temp2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
				if (temp < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::A, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = temp;
						diff_min2 = temp2;
					}
				}
			}
			//if (choice != NULL) {
			//	choice->notUseDay = xuniji.end_day;
			//}

		}
	}
	else {
		//双节点,策略是选AB加起来比例最接近的,且差不多的
		int target_memory = xuniji.neicun / 2;
		int target_core = xuniji.neihe / 2;
		double diff_min1 = 100000.0; //比例距离
		double diff_min2 = 100000.0;
		//1.2.1
		for (auto& p : fuwuqi_list) {
			//if (p->notUseDay != GlobalMessage::Get().all_day) {
			//	continue;
			//}
			if (!p->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			//先淘汰内存不足的
			if (p->restNeiheA() < target_core || p->restNeiheB() < target_core ||
				p->restNeicunA() < target_memory || p->restNeicunB() < target_memory) {
				continue;
			}
			double memory_core_rate = p->restNeicunNeiheRate();
			double temp = abs(memory_core_rate - xuniji_memory_core_rate);
			double temp2 = (1.0*p->restNeiheA() + p->restNeiheB()) / xuniji.neihe + (1.0 * p->restNeicunA() + p->restNeicunB()) / xuniji.neicun;
			if (temp < (diff_min1 - MIN_DIFF)) {
				optimal_machine = make_pair(AB::AB, p);
				diff_min1 = temp;
				diff_min2 = temp2;
			}
			else if (temp < (diff_min1 + MIN_DIFF)) {
				if (temp2 < diff_min2) {
					optimal_machine = make_pair(AB::AB, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
			}
		}

		//如果还没有找到,那就分配没有使用的服务器 1.2.2
		if (optimal_machine.second == nullptr) {
			//double xingjiabi = -1;
			double diff_min1 = 100000.0; //比例距离
			double diff_min2 = 100000.0;
			for (auto& p : fuwuqi_list) {
				if (p->isUsed()) {
					continue;//选没有使用的服务器
				}
				if (p->restNeiheA() < target_core || p->restNeicunA() < target_memory) {
					continue;
				}
				if (p->restNeiheB() < target_core || p->restNeicunB() < target_memory) {
					continue;
				}
				//double temp = (xuniji_future_memory_core_rate_balance * p->neihe + p->neicun) / (1.0*xuniji.end_day - currentDay) * (p->costOneDay);
				//if (temp > xingjiabi) {
				//	xingjiabi = temp;
				//	//p->notUseDay = xuniji.end_day;
				//	optimal_machine = make_pair(AB::AB, p);
				//}
				double memory_core_rate = p->restNeicunNeiheRate();
				double temp = abs(memory_core_rate - xuniji_memory_core_rate);
				double temp2 = (double)p->restNeihe() / xuniji.neihe + (double)p->restNeicun() / xuniji.neicun;
				if (temp < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::AB, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						optimal_machine = make_pair(AB::AB, p);
						diff_min1 = temp;
						diff_min2 = temp2;
					}
				}
			}
		}
	}


	return optimal_machine;
}

pair<MachineLibarary::AB, shared_ptr<FUWUQI>> MachineLibarary::findOptimalFuWuQi_huachuang_qianyi(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& in_it)
{

	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> optimal_machine;//选取的最优机器
	int currentDay = GlobalMessage::Get().current_Day; //当前日期
	//遍历所有机器，找比例最接近的节点
	if (xuniji.shuangjiedian == false) {
		//priority_queue < shared_ptr<FUWUQI>, vector<shared_ptr<FUWUQI>>,> min_dui;

		for (auto& p : in_it) {
			//先从服务器闲置日期>=虚拟机释放日期的服务器中分配
			int shi_heng = p->shiHeng();//shi_heng表示是不是A比B多太多了？要保持AB的平衡
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
				optimal_machine = make_pair(AB::A, p);
			}
			else if (shi_heng == 1) {
				//选取B
				if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
					continue;
				}
				optimal_machine = make_pair(AB::B, p);
			}
			else {
				//A和B都参与竞选
				optimal_machine = make_pair(AB::A, p);
			}
		}

	}
	else {
		//双节点,策略是选AB加起来比例最接近的,且差不多的
		int target_memory = xuniji.neicun / 2;
		int target_core = xuniji.neihe / 2;
		double diff_min1 = 100000.0; //比例距离
		double diff_min2 = 100000.0;

		for (auto& p : in_it) {
			//先淘汰内存不足的
			if (p->restNeiheA() < target_core || p->restNeiheB() < target_core ||
				p->restNeicunA() < target_memory || p->restNeicunB() < target_memory) {
				continue;
			}
			optimal_machine = make_pair(AB::AB, p);
		}
	}
	return optimal_machine;
}


pair<MachineLibarary::AB, shared_ptr<FUWUQI>> MachineLibarary::findOptimalFuWuQi_fenqu_qianyi(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& unused_set, unordered_set<shared_ptr<FUWUQI>>& in_it) {
	//1.2策略 分区迁移
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> optimal_machine;//最优机器

	double xuniji_memory_core_rate = (double)xuniji.neicun / (double)xuniji.neihe;
	double core = 1.0 * xuniji.neihe + GlobalMessage::Get().all_unadd_core;
	double memory = 1.0 * xuniji.neicun + GlobalMessage::Get().all_unadd_memory;
	double xuniji_future_memory_core_rate_balance = 1.0 * memory / core;

	int currentDay = GlobalMessage::Get().current_Day; //当前日期
	//遍历所有机器，找比例最接近的节点
	if (xuniji.shuangjiedian == false) {
		//priority_queue < shared_ptr<FUWUQI>, vector<shared_ptr<FUWUQI>>,> min_dui;
		double diff_min1 = 100000;
		double diff_min2 = 100000;
		//单节点
		//unused_set.empty 表示插入 else 表示迁移

		for (auto& p : in_it) {
			if (unused_set.find(p) != unused_set.end()) continue;//如果机器已经迁移过，那就直接跳过
			if (!p->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			//先从服务器闲置日期>=虚拟机释放日期的服务器中分配
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
				if (temp < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::A, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = temp;
						diff_min2 = temp2;
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
				if (temp < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::B, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						optimal_machine = make_pair(AB::B, p);
						diff_min1 = temp;
						diff_min2 = temp2;

					}
				}
			}
			else {
				//A和B都参与竞选
				double tempA = abs(memory_core_rateA - xuniji_memory_core_rate);
				double tempA2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
				if (tempA < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::A, p);
					diff_min1 = tempA;
					diff_min2 = tempA2;

				}
				else if (tempA < (diff_min1 + MIN_DIFF)) {
					if (tempA2 < diff_min2) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = tempA;
						diff_min2 = tempA2;

					}
				}
				double tempB = abs(memory_core_rateB - xuniji_memory_core_rate);
				double tempB2 = (double)p->restNeiheB() / xuniji.neihe + (double)p->restNeicunB() / xuniji.neicun;
				if (tempB < (diff_min1 - MIN_DIFF)) {
					optimal_machine = make_pair(AB::B, p);
					diff_min1 = tempB;
					diff_min2 = tempB2;

				}
				else if (tempB < (diff_min1 + MIN_DIFF)) {
					if (tempB2 < diff_min2) {
						optimal_machine = make_pair(AB::B, p);
						diff_min1 = tempB;
						diff_min2 = tempB2;

					}
				}
			}
		}

	}
	else {
		//双节点,策略是选AB加起来比例最接近的,且差不多的
		int target_memory = xuniji.neicun / 2;
		int target_core = xuniji.neihe / 2;
		double diff_min1 = 100000.0; //比例距离
		double diff_min2 = 100000.0;

		for (auto& p : in_it) {
			if (unused_set.find(p) != unused_set.end()) continue;//如果机器已经迁移过，那就直接跳过
			if (!p->isUsed()) {
				continue;//优先选正在使用的服务器
			}
			//先淘汰内存不足的
			if (p->restNeiheA() < target_core || p->restNeiheB() < target_core ||
				p->restNeicunA() < target_memory || p->restNeicunB() < target_memory) {
				continue;
			}
			double memory_core_rate = p->restNeicunNeiheRate();
			double temp = abs(memory_core_rate - xuniji_memory_core_rate);
			double temp2 = (double)p->restNeihe() / xuniji.neihe + (double)p->restNeicun() / xuniji.neicun;
			if (temp < (diff_min1 - MIN_DIFF)) {
				optimal_machine = make_pair(AB::AB, p);
				diff_min1 = temp;
				diff_min2 = temp2;

			}
			else if (temp < (diff_min1 + MIN_DIFF)) {
				if (((double)p->restNeihe() / xuniji.neihe + (double)p->restNeicun() / xuniji.neicun) < diff_min2) {
					optimal_machine = make_pair(AB::AB, p);
					diff_min1 = temp;
					diff_min2 = temp2;
				}
			}
		}
	}
	return optimal_machine;
}

pair<MachineLibarary::AB, shared_ptr<FUWUQI>> MachineLibarary::findOptimalFuWuQi_fenqu_qianyi_2(XUNIJI& xuniji, unordered_set<shared_ptr<FUWUQI>>& unused_set, unordered_set<shared_ptr<FUWUQI>>& in_it)
{
	pair<MachineLibarary::AB, shared_ptr<FUWUQI>> optimal_machine;//最优机器

	double xuniji_memory_core_rate = (double)xuniji.neicun / (double)xuniji.neihe;
	double core = 1.0 * xuniji.neihe + GlobalMessage::Get().all_unadd_core;
	double memory = 1.0 * xuniji.neicun + GlobalMessage::Get().all_unadd_memory;
	double xuniji_future_memory_core_rate_balance = 1.0 * memory / core;

	int currentDay = GlobalMessage::Get().current_Day; //当前日期

	if (true) {
	
	}
	//如果知道该虚拟机的释放日期
	else {
		if (xuniji.shuangjiedian == false) {
			//priority_queue < shared_ptr<FUWUQI>, vector<shared_ptr<FUWUQI>>,> min_dui;
			double diff_min1 = 100000;
			double diff_min2 = 100000;
			//单节点
			//2.1.1
			for (auto& p : fuwuqi_list) {
				//if (p->notUseDay == GlobalMessage::Get().all_day) {
				//	continue;
				//}
				if (!p->isUsed()) {
					continue;//优先选正在使用的服务器
				}
				//if (p->notUseDay < xuniji.end_day) {
				//	continue;
				//}
				int shi_heng = p->shiHeng();//shi_heng表示是不是A比B多太多了？要保持AB的平衡
				//如果A空间不足，就只判断B，如果B空间不足，就只判断A
				if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
					shi_heng = 1;
				}
				if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
					shi_heng = 0;
				}

				double memory_core_rateA = p->restNeicunNeiheRateA();
				double memory_core_rateB = p->restNeicunNeiheRateB();
				if (shi_heng == 0) {
					//如果A使用太少了，就选取A
					//判断是否有剩余空间

					if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
						continue;
					}
					double temp = abs(memory_core_rateA - xuniji_memory_core_rate);
					double temp2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
					if (temp < (diff_min1 - MIN_DIFF)) {
						diff_min1 = temp;
						diff_min2 = temp2;
						optimal_machine = make_pair(AB::A, p);
					}
					else if (temp < (diff_min1 + MIN_DIFF)) {
						if (temp2 < diff_min2) {
							diff_min1 = temp;
							diff_min2 = temp2;
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
					if (temp < (diff_min1 - MIN_DIFF)) {
						optimal_machine = make_pair(AB::B, p);
						diff_min1 = temp;
						diff_min2 = temp2;

					}
					else if (temp < (diff_min1 + MIN_DIFF)) {
						if (temp2 < diff_min2) {
							optimal_machine = make_pair(AB::B, p);
							diff_min1 = temp;
							diff_min2 = temp2;

						}
					}
				}
				else {

					//A和B都参与竞选
					double tempA = abs(memory_core_rateA - xuniji_memory_core_rate);
					double tempA2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
					if (tempA < (diff_min1 - MIN_DIFF)) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = tempA;
						diff_min2 = tempA2;

					}
					else if (tempA < (diff_min1 + MIN_DIFF)) {
						if (tempA2 < diff_min2) {
							optimal_machine = make_pair(AB::A, p);
							diff_min1 = tempA;
							diff_min2 = tempA2;

						}
					}
					double tempB = abs(memory_core_rateB - xuniji_memory_core_rate);
					double tempB2 = (double)p->restNeiheB() / xuniji.neihe + (double)p->restNeicunB() / xuniji.neicun;
					if (tempB < (diff_min1 - MIN_DIFF)) {
						optimal_machine = make_pair(AB::B, p);
						diff_min1 = tempB;
						diff_min2 = tempB2;

					}
					else if (tempB < (diff_min1 + MIN_DIFF)) {
						if (tempB2 < diff_min2) {
							optimal_machine = make_pair(AB::B, p);
							diff_min1 = tempB;
							diff_min2 = tempB2;
						}
					}
				}
			}
			//2.1.2
			if (optimal_machine.second == nullptr) {
				map<int, std::shared_ptr<FUWUQI>> fuwuqi_mp;
				list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
				for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
					//不插入
					//if ((*fuwuqi_ptr)->notUseDay == GlobalMessage::Get().all_day) {
					//	continue;
					//}
					if (!(*fuwuqi_ptr)->isUsed()) {
						continue;//优先选正在使用的服务器
					}

					//if ((*fuwuqi_ptr)->notUseDay < xuniji.end_day) {
					//	fuwuqi_mp[(*fuwuqi_ptr)->notUseDay] = *fuwuqi_ptr;
					//}
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
						//riter->second->notUseDay = xuniji.end_day;
						optimal_machine = make_pair(AB::A, riter->second);
						break;
					}
					else if (shi_heng == 1) {
						//选取B
						if (riter->second->restNeiheB() < xuniji.neihe || riter->second->restNeicunB() < xuniji.neicun) {
							continue;
						}
						//riter->second->notUseDay = xuniji.end_day;
						optimal_machine = make_pair(AB::B, riter->second);
						break;
					}
					else {
						//riter->second->notUseDay = xuniji.end_day;
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
			//如果还没有找到,那就分配没有使用的服务器2.1.3
			if (optimal_machine.second == nullptr) {
				//double xingjiabi = -1;
				double diff_min1 = 100000;
				double diff_min2 = 100000;
				std::shared_ptr<FUWUQI> choice;
				for (auto& p : fuwuqi_list) {
					if (p->isUsed()) {
						continue;//选没有使用的服务器
					}
					if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
						continue;
					}
					if (p->restNeiheB() < xuniji.neihe || p->restNeicunB() < xuniji.neicun) {
						continue;
					}
					//if ((xuniji_future_memory_core_rate_balance * p->neihe + p->neicun) / (((double)xuniji.end_day - currentDay) * (p->costOneDay)) > xingjiabi) {
					//	xingjiabi = (xuniji_future_memory_core_rate_balance * p->neihe + p->neicun) / (((double)xuniji.end_day - currentDay) * (p->costOneDay));
					//	optimal_machine = make_pair(AB::A, p);
					//}
					double memory_core_rateA = p->restNeicunNeiheRateA();
					if (p->restNeiheA() < xuniji.neihe || p->restNeicunA() < xuniji.neicun) {
						continue;
					}
					double temp = abs(memory_core_rateA - xuniji_memory_core_rate);
					double temp2 = (double)p->restNeiheA() / xuniji.neihe + (double)p->restNeicunA() / xuniji.neicun;
					if (temp < (diff_min1 - MIN_DIFF)) {
						optimal_machine = make_pair(AB::A, p);
						diff_min1 = temp;
						diff_min2 = temp2;

						choice = p;
					}
					else if (temp < (diff_min1 + MIN_DIFF)) {
						if (temp2 < diff_min2) {
							optimal_machine = make_pair(AB::A, p);
							diff_min1 = temp;
							diff_min2 = temp2;

							choice = p;
						}
					}
				}
				if (choice != nullptr) {
					//choice->notUseDay = xuniji.end_day;
				}
			}
		}
		else {
			//双节点,策略是选AB加起来比例最接近的,且差不多的
			int target_memory = xuniji.neicun / 2;
			int target_core = xuniji.neihe / 2;
			double diff_min1 = 100000.0; //比例距离
			double diff_min2 = 100000.0;
			//2.2.1
			for (auto& p : fuwuqi_list) {
				//不插入
				//if (p->notUseDay == GlobalMessage::Get().all_day) {
				//	continue;
				//}
				if (!p->isUsed()) {
					continue;//优先选正在使用的服务器
				}
				//if (p->notUseDay < xuniji.end_day) {
				//	continue;
				//}
				//先淘汰内存不足的
				if (p->restNeiheA() < target_core || p->restNeiheB() < target_core ||
					p->restNeicunA() < target_memory || p->restNeicunB() < target_memory) {
					continue;
				}
				double memory_core_rate = p->restNeicunNeiheRate();
				double temp = abs(memory_core_rate - xuniji_memory_core_rate);
				double temp2 = (double)(p->restNeiheA() + p->restNeiheB()) / xuniji.neihe + (double)(p->restNeicunA() + p->restNeicunB()) / xuniji.neicun;
				if (temp < (diff_min1 - MIN_DIFF)) {
					diff_min1 = temp;
					diff_min2 = temp2;
					optimal_machine = make_pair(AB::AB, p);
				}
				else if (temp < (diff_min1 + MIN_DIFF)) {
					if (temp2 < diff_min2) {
						diff_min1 = temp;
						diff_min2 = temp2;
						optimal_machine = make_pair(AB::AB, p);
					}
				}
			}
			//如果没有找到2.2.2
			if (optimal_machine.second == nullptr) {
				map<int, std::shared_ptr<FUWUQI>> fuwuqi_mp;
				list<std::shared_ptr<FUWUQI>>::iterator fuwuqi_ptr;
				for (fuwuqi_ptr = fuwuqi_list.begin(); fuwuqi_ptr != fuwuqi_list.end(); fuwuqi_ptr++) {
					//不插入
					//if ((*fuwuqi_ptr)->notUseDay == GlobalMessage::Get().all_day) {
					//	continue;
					//}
					if (!(*fuwuqi_ptr)->isUsed()) {
						continue;//优先选正在使用的服务器
					}
					//if ((*fuwuqi_ptr)->notUseDay < xuniji.end_day) {
					//	fuwuqi_mp[(*fuwuqi_ptr)->notUseDay] = (*fuwuqi_ptr);
					//}
				}
				map<int, std::shared_ptr<FUWUQI>>::reverse_iterator riter = fuwuqi_mp.rbegin();//从后面往前面插
				for (riter; riter != fuwuqi_mp.rend(); riter++) {
					//先淘汰内存不足的
					if (riter->second->restNeiheA() < target_core || riter->second->restNeiheB() < target_core ||
						riter->second->restNeicunA() < target_memory || riter->second->restNeicunB() < target_memory) {
						continue;
					}
					//riter->second->notUseDay = xuniji.end_day;
					optimal_machine = make_pair(AB::AB, riter->second);
					break;
				}
			}
			//如果还没有找到,那就分配没有使用的服务器2.2.3
			if (optimal_machine.second == nullptr) {
				//double xingjiabi = -1;
				std::shared_ptr<FUWUQI> choice;
				for (auto& p : fuwuqi_list) {
					//不插入
					if (p->isUsed()) {
						continue;//选没有使用的服务器
					}

					if (p->restNeiheA() < target_core || p->restNeicunA() < target_memory) {
						continue;
					}
					if (p->restNeiheB() < target_core || p->restNeicunB() < target_memory) {
						continue;
					}
					//double temp = (xuniji_future_memory_core_rate_balance * p->neihe + p->neicun) / (1.0*xuniji.end_day - currentDay) * (p->costOneDay);
					//if (temp > xingjiabi) {
					//	xingjiabi = temp;
					//	//p->notUseDay = xuniji.end_day;
					//	optimal_machine = make_pair(AB::AB, p);
					//}
					double memory_core_rate = p->restNeicunNeiheRate();
					double temp = abs(memory_core_rate - xuniji_memory_core_rate);
					double temp2 = (double)p->restNeihe() / xuniji.neihe + (double)p->restNeicun() / xuniji.neicun;
					if (temp < (diff_min1 - MIN_DIFF)) {
						diff_min1 = temp;
						diff_min2 = temp2;
						optimal_machine = make_pair(AB::AB, p);
						choice = p;
					}
					else if (temp < (diff_min1 + MIN_DIFF)) {
						if (temp2 < diff_min2) {
							diff_min1 = temp;
							diff_min2 = temp2;
							optimal_machine = make_pair(AB::AB, p);
							choice = p;
						}
					}
				}
				if (choice != nullptr) {
					//choice->notUseDay = xuniji.end_day;
				}
			}
		}
	}
	return optimal_machine;
}




//有BUG要改
void GlobalMessage::CalXunijiWillContinueDay(int start, int end) {
	for (int i = start; i <= end; i++) {
		for (int j = 0; j < Get().op_list_vec[i].size(); ++j) {
			LogicalOperation& temp_op = Get().op_list_vec[i][j];
			if (temp_op.op_name == "add") {
				Get().allneed_xinuji_map[temp_op.xuniji_id] = Get().xinuji_map[temp_op.xing_hao];
				Get().allneed_xinuji_map[temp_op.xuniji_id].end_day = Get().all_day;
				Get().allneed_xinuji_map[temp_op.xuniji_id].start_day = i;
				Get().add_op[i]++;
			}
			else if (temp_op.op_name == "del") {
				//Get().allneed_xinuji_map[temp_op.xuniji_id].continue_day = i - Get().allneed_xinuji_map[temp_op.xuniji_id].start_day;
				if (MachineLibarary::get_instance()->id_xuniji_dict.find(temp_op.xuniji_id) != MachineLibarary::get_instance()->id_xuniji_dict.end()) {
					//MachineLibarary::get_instance()->id_xuniji_dict[temp_op.xuniji_id].continue_day = i - MachineLibarary::get_instance()->id_xuniji_dict[temp_op.xuniji_id].start_day;
					MachineLibarary::get_instance()->id_xuniji_dict[temp_op.xuniji_id].end_day = i;
				}
				Get().allneed_xinuji_map[temp_op.xuniji_id].end_day = i;
				Get().del_op[i]++;
			}
		}
	}
}

FUWUQI Buy::buyOptimalMachine2(const XUNIJI& xuniji) {
	//1.1策略：根据没有分配的虚拟机决定性价比参数;
	int current_day = GlobalMessage::Get().current_Day;
	double memory = (double)xuniji.neicun + GlobalMessage::Get().all_unadd_memory;
	double core = (double)xuniji.neihe + GlobalMessage::Get().all_unadd_core;
	double xuniji_memory_core_rate = 1.0 * memory / core;
	double max_xingjiabi = -1;//最大性价比
	FUWUQI res;
	double neiheliyonglv = 1.0 * MachineLibarary::get_instance()->getNeiheliyonglv();
	double neicunliyonglv = 1.0 * MachineLibarary::get_instance()->getNeicunliyonglv();
	double now_neiheneicunbili = neiheliyonglv / neicunliyonglv;
	for (FUWUQI& p : GlobalMessage::Get().fuwuqi_vec) {
		if (xuniji.shuangjiedian == true && (xuniji.neihe > p.neihe || xuniji.neicun > p.neicun)) continue;//无法满足要求
		if (xuniji.shuangjiedian == false && (xuniji.neihe > p.neihe / 2 || xuniji.neicun > p.neicun / 2)) continue;//无法满足要求
		double x = CalFuwuqiScore(p, xuniji_memory_core_rate, now_neiheneicunbili);
		if (x > max_xingjiabi) {
			max_xingjiabi = x;
			res = p;
		}
	}
	return res;
}

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
	void MigrationAdd(vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>>& temp);
	void FuWuQiAdd(string fuwuqi_xinghao, shared_ptr<FUWUQI> fuwuqi_ptr);
	vector<WUFUQI_AB> bushu_vec;//当operate==bushu,一天部署的虚拟机<服务器id,结点>
	vector<pair<string, WUFUQI_AB>> migration_vec;//当operate==migration,一天迁移的虚拟机id和服务器id,结点
	map<string, vector<shared_ptr<FUWUQI>>> add_fuwuqi_map;//当operate=="purchase" 一天购买的服务器型号和对应的服务器指针
	void Print();
};
void Printer::BuShu(WUFUQI_AB fuwuqi) {
	this->bushu_vec.push_back(fuwuqi);
}
void Printer::MigrationAdd(vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>>& temp) {
	for (int i = 0; i < temp.size(); i++) {
		WUFUQI_AB a(int(temp[i].second.first), temp[i].second.second);
		migration_vec.push_back(make_pair(temp[i].first, a));
	}
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
	printf("(migration,%d)\n", (int)this->migration_vec.size());
	for (pair<string, WUFUQI_AB>& migration_iter : this->migration_vec) {
		if (migration_iter.second.AB == 3) {
			printf("(%s,%d)\n", migration_iter.first.c_str(), migration_iter.second.fuwuqi_ptr->id);

		}
		else if (migration_iter.second.AB == 1) {
			printf("(%s,%d,A)\n", migration_iter.first.c_str(), migration_iter.second.fuwuqi_ptr->id);

		}
		else if (migration_iter.second.AB == 2) {
			printf("(%s,%d,B)\n", migration_iter.first.c_str(), migration_iter.second.fuwuqi_ptr->id);

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
//
//void GlobalMessage::Set(string filepath) {
//	Buy b;
//	//初始化数据
//	Get().current_Day = 0;
//	Get().all_cost = 0;
//	Get().all_unadd_core = 0;
//	Get().all_unadd_memory = 0;
//	Get().xuniji_count = 0;
//	Get().fuwuqi_id = 0;
//	//读取文件
//	ifstream infile;
//	infile.open(filepath);
//	if (!infile.is_open())
//	{
//		cout << "cannot find file" << endl;
//		return;
//	}
//	string temp;
//	getline(infile, temp);
//	int Wtype;
//	int Vtype;
//	int All_Day;
//	int Window;
//	//读取服务器数据
//	Wtype = stoi(temp);
//	for (int i = 0; i < Wtype; i++) {
//		getline(infile, temp);
//		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
//		vector<string> temp_str_vec;
//		splitString(temp, ",", temp_str_vec);
//		FUWUQI f = FUWUQI(temp_str_vec[0], stoi(temp_str_vec[1]), stoi(temp_str_vec[2]), stoi(temp_str_vec[3]), stoi(temp_str_vec[4]), -1);
//		Get().fuwuqi_vec.push_back(f);
//	}
//	getline(infile, temp);
//	Vtype = stoi(temp);
//	//读取虚拟机数据
//	for (int i = 0; i < Vtype; i++) {
//		getline(infile, temp);
//		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
//		vector<string> strDest;
//		splitString(temp, ",", strDest);
//		XUNIJI x = XUNIJI(strDest[0], stoi(strDest[1]), stoi(strDest[2]), (bool)stoi(strDest[3]));
//		Get().xinuji_map[strDest[0]] = x;
//	}
//	getline(infile, temp);
//	vector<string> tempStrDest;
//	splitString(temp, " ", tempStrDest);
//	All_Day = stoi(tempStrDest[0]);
//	Window = stoi(tempStrDest[1]);
//	Get().all_day = All_Day;
//	Get().forward_day = Window;
//	//int countadd = 0;
//	//int countdel = 0;
//	for (int i = 0; i < All_Day + Window; i++) {
//		//记录前window天数据
//		if (i < Window) {
//			getline(infile, temp);
//			int All_Require = stoi(temp);
//			vector<LogicalOperation> op_list;
//			for (int j = 0; j < All_Require; j++) {
//				getline(infile, temp);
//				temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
//				vector<string> strDest;
//				splitString(temp, ",", strDest);
//				if (strDest[0] == "add") {
//					LogicalOperation add_op(strDest[0], trim(strDest[1]), trim(strDest[2]));
//					op_list.push_back(add_op);
//					Get().all_unadd_core += Get().xinuji_map[trim(strDest[1])].neihe;
//					Get().all_unadd_memory += Get().xinuji_map[trim(strDest[1])].neicun;
//				}
//				else {
//					LogicalOperation del_op(strDest[0], trim(strDest[1]));
//					GlobalMessage::Get().all_unadd_memory -= Get().allneed_xinuji_map[trim(strDest[1])].neicun;
//					GlobalMessage::Get().all_unadd_core -= Get().allneed_xinuji_map[trim(strDest[1])].neihe;
//					op_list.push_back(del_op);
//				}
//			}
//			Get().op_list_vec.push_back(op_list);
//		}
//		if (i == Window) {
//			CalXunijiWillContinueDay(0, Window - 1);
//		}
//		// 输出当天数据  记录当前当天window天后的数据
//		if (i >= Window && i < All_Day) {
//			cout << "core:" << MachineLibarary::get_instance()->getNeiheliyonglv() << endl;
//			cout << "memory:" << MachineLibarary::get_instance()->getNeicunliyonglv() << endl;
//			Printer printer;
//			vector<LogicalOperation>& op_vec = Get().op_list_vec[Get().current_Day];
//			vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> migration_vec = MachineLibarary::get_instance()->migaration();
//			cout << "core:" << MachineLibarary::get_instance()->getNeiheliyonglv() << endl;
//			cout << "memory:" << MachineLibarary::get_instance()->getNeicunliyonglv() << endl;
//			printer.MigrationAdd(migration_vec);
//			int addcount = 0;
//			for (auto& temp_op : op_vec) {
//
//				if (temp_op.op_name == "add") {
//					XUNIJI& xuniji = GlobalMessage::Get().allneed_xinuji_map[temp_op.xuniji_id];
//					unordered_set<shared_ptr<FUWUQI>> tmp_set;
//					unordered_set<shared_ptr<FUWUQI>> tmp_init;
//					pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > p = MachineLibarary::get_instance()->addXuNiJi(temp_op.xuniji_id, xuniji, tmp_set, tmp_init);
//					if (p.second == NULL) {
//						addcount++;
//						FUWUQI fuwuqi = b.buyOptimalMachine2(xuniji);
//						Get().all_cost += fuwuqi.cost;
//						std::shared_ptr<FUWUQI> new_fuwuqi_ptr = MachineLibarary::get_instance()->addFuWuQi(fuwuqi);
//						//new_fuwuqi_ptr->notUseDay = xuniji.end_day;
//						printer.FuWuQiAdd(fuwuqi.xinghao, new_fuwuqi_ptr);
//						if (xuniji.shuangjiedian) {
//							MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::AB, new_fuwuqi_ptr, temp_op.xuniji_id, xuniji);
//							p = make_pair(MachineLibarary::AB::AB, new_fuwuqi_ptr);
//						}
//						else {
//							MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::A, new_fuwuqi_ptr, temp_op.xuniji_id, xuniji);
//							p = make_pair(MachineLibarary::AB::A, new_fuwuqi_ptr);
//						}
//					}
//					WUFUQI_AB wufuqi_ab((int)p.first, p.second);
//					printer.BuShu(wufuqi_ab);
//					GlobalMessage::Get().xuniji_count++;
//				}
//				else if (temp_op.op_name == "del") {
//					GlobalMessage::Get().xuniji_count--;
//					MachineLibarary::get_instance()->delXuNiJi(temp_op.xuniji_id);
//				}
//			}
//			cout << "add:" << Get().add_op[Get().current_Day] << endl;
//			cout << "del:" << Get().del_op[Get().current_Day] << endl;
//			cout << "unused:" << MachineLibarary::get_instance()->getUnUsed() << "/" << MachineLibarary::get_instance()->getALL() << endl;
//			cout << "core:" << MachineLibarary::get_instance()->getNeiheliyonglv() << endl;
//			cout << "memory:" << MachineLibarary::get_instance()->getNeicunliyonglv() << endl;
//			Get().all_cost += MachineLibarary::get_instance()->getCurrentCost();
//
//			//if (Get().all_cost < 0) {
//			//	cout << Get().all_cost << ":" << MachineLibarary::get_instance()->getCurrentCost() << "error!" << endl;
//			//}
//
//			cout << Get().current_Day << ":" << Get().all_cost << endl;
//			Get().current_Day++;
//
//			//读
//			getline(infile, temp);
//			int All_Require = stoi(temp);
//			vector<LogicalOperation> op_list;
//			for (int j = 0; j < All_Require; j++) {
//				getline(infile, temp);
//				temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
//				vector<string> strDest;
//				splitString(temp, ",", strDest);
//				if (strDest[0] == "add") {
//					LogicalOperation add_op(strDest[0], trim(strDest[1]), trim(strDest[2]));
//					op_list.push_back(add_op);
//					Get().all_unadd_core += Get().xinuji_map[trim(strDest[1])].neihe;
//					Get().all_unadd_memory += Get().xinuji_map[trim(strDest[1])].neicun;
//				}
//				else {
//					LogicalOperation del_op(strDest[0], trim(strDest[1]));
//					op_list.push_back(del_op);
//					GlobalMessage::Get().all_unadd_memory -= Get().allneed_xinuji_map[trim(strDest[1])].neicun;
//					GlobalMessage::Get().all_unadd_core -= Get().allneed_xinuji_map[trim(strDest[1])].neihe;
//				}
//			}
//			Get().op_list_vec.push_back(op_list);
//			CalXunijiWillContinueDay(i, i);
//		}
//		// 输出当天数据
//		else if (i >= All_Day) {
//			Printer printer;
//			vector<LogicalOperation>& op_vec = Get().op_list_vec[Get().current_Day];
//			vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> migration_vec = MachineLibarary::get_instance()->migaration();
//			printer.MigrationAdd(migration_vec);
//			int addcount = 0;
//			for (auto& temp_op : op_vec) {
//
//				if (temp_op.op_name == "add") {
//					XUNIJI& xuniji = GlobalMessage::Get().allneed_xinuji_map[temp_op.xuniji_id];
//					unordered_set<shared_ptr<FUWUQI>> tmp_set;
//					unordered_set<shared_ptr<FUWUQI>> tmp_init;
//					pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > p = MachineLibarary::get_instance()->addXuNiJi(temp_op.xuniji_id, xuniji, tmp_set, tmp_init);
//					if (p.second == NULL) {
//						addcount++;
//						FUWUQI fuwuqi = b.buyOptimalMachine2(xuniji);
//						Get().all_cost += fuwuqi.cost;
//						std::shared_ptr<FUWUQI> new_fuwuqi_ptr = MachineLibarary::get_instance()->addFuWuQi(fuwuqi);
//						//new_fuwuqi_ptr->notUseDay = xuniji.end_day;
//
//						printer.FuWuQiAdd(fuwuqi.xinghao, new_fuwuqi_ptr);
//						if (xuniji.shuangjiedian) {
//							MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::AB, new_fuwuqi_ptr, temp_op.xuniji_id, xuniji);
//							p = make_pair(MachineLibarary::AB::AB, new_fuwuqi_ptr);
//						}
//						else {
//							MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::A, new_fuwuqi_ptr, temp_op.xuniji_id, xuniji);
//							p = make_pair(MachineLibarary::AB::A, new_fuwuqi_ptr);
//						}
//
//					}
//					WUFUQI_AB wufuqi_ab((int)p.first, p.second);
//					printer.BuShu(wufuqi_ab);
//					GlobalMessage::Get().xuniji_count++;
//				}
//				else if (temp_op.op_name == "del") {
//					GlobalMessage::Get().xuniji_count--;
//					MachineLibarary::get_instance()->delXuNiJi(temp_op.xuniji_id);
//				}
//			}
//			//cout << "add:" << addcount << endl;
//			cout << "unused:" << MachineLibarary::get_instance()->getUnUsed() << "/" << MachineLibarary::get_instance()->getALL() << endl;
//			cout << "core:" << MachineLibarary::get_instance()->getNeiheliyonglv() << endl;
//			cout << "memory:" << MachineLibarary::get_instance()->getNeicunliyonglv() << endl;
//			Get().all_cost += MachineLibarary::get_instance()->getCurrentCost();
//
//			//if (Get().all_cost < 0) {
//			//	cout << Get().all_cost << ":" << MachineLibarary::get_instance()->getCurrentCost() << "error!" << endl;
//			//}
//
//			cout << Get().current_Day << ":" << Get().all_cost << endl;
//			Get().current_Day++;
//		}
//	}
//
//
//}

void GlobalMessage::dealOneday(){
	Buy b;
	Printer printer;
	vector<LogicalOperation>& op_vec = Get().op_list_vec[Get().current_Day];
	vector<pair<string, pair<MachineLibarary::AB, std::shared_ptr<FUWUQI>>>> migration_vec = MachineLibarary::get_instance()->migaration();
	printer.MigrationAdd(migration_vec);
	for (auto& temp_op : op_vec) {
		if (temp_op.op_name == "add") {
			unordered_set<shared_ptr<FUWUQI>> tmp_set;
			unordered_set<shared_ptr<FUWUQI>> tmp_init;
			XUNIJI& xuniji = GlobalMessage::Get().allneed_xinuji_map[temp_op.xuniji_id];
			pair<MachineLibarary::AB, std::shared_ptr<FUWUQI> > p = MachineLibarary::get_instance()->addXuNiJi(temp_op.xuniji_id, xuniji, tmp_set, tmp_init);
			if (p.second == NULL) {
				FUWUQI fuwuqi = b.buyOptimalMachine2(xuniji);
				//Get().all_cost += fuwuqi.cost;
				std::shared_ptr<FUWUQI> new_fuwuqi_ptr = MachineLibarary::get_instance()->addFuWuQi(fuwuqi);
				//new_fuwuqi_ptr->notUseDay = xuniji.end_day;
				printer.FuWuQiAdd(fuwuqi.xinghao, new_fuwuqi_ptr);
				if (xuniji.shuangjiedian) {
					MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::AB, new_fuwuqi_ptr, temp_op.xuniji_id, xuniji);
					p = make_pair(MachineLibarary::AB::AB, new_fuwuqi_ptr);
				}
				else {
					MachineLibarary::get_instance()->addXuNiJi(MachineLibarary::AB::A, new_fuwuqi_ptr, temp_op.xuniji_id, xuniji);
					p = make_pair(MachineLibarary::AB::A, new_fuwuqi_ptr);
				}
			}
			WUFUQI_AB wufuqi_ab((int)p.first, p.second);
			printer.BuShu(wufuqi_ab);
			GlobalMessage::Get().xuniji_count++;
		}
		else if (temp_op.op_name == "del") {
			GlobalMessage::Get().xuniji_count--;
			MachineLibarary::get_instance()->delXuNiJi(temp_op.xuniji_id);
		}
	}
	//Get().all_cost += MachineLibarary::get_instance()->getCurrentCost();
	//cout << Get().current_Day << ":" << Get().all_cost << endl;
	printer.Print();
}

void GlobalMessage::Init() {
	Get().current_Day = 0;
	Get().all_cost = 0;
	Get().all_unadd_core = 0;
	Get().all_unadd_memory = 0;
	Get().xuniji_count = 0;
	int N;
	cin >> N;
	string te = "\n";
	getline(cin, te);
	for (int i = 0; i < N; i++) {
		char szBuf[256];
		cin.getline(szBuf, 256);
		string temp(szBuf);
		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
		vector<string> temp_str_vec;
		splitString(temp, ",", temp_str_vec);
		FUWUQI f = FUWUQI(temp_str_vec[0], stoi(temp_str_vec[1]), stoi(temp_str_vec[2]), stoi(temp_str_vec[3]), stoi(temp_str_vec[4]), -1);
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
		vector<string> temp_str_vec;
		splitString(temp, ",", temp_str_vec);
		XUNIJI x = XUNIJI(temp_str_vec[0], stoi(temp_str_vec[1]), stoi(temp_str_vec[2]), (bool)stoi(temp_str_vec[3]));
		Get().xinuji_map[temp_str_vec[0]] = x;
	}
	int All_Day;
	int forward_day;
	cin >> All_Day;
	cin >> forward_day;
	Get().all_day = All_Day;
	Get().forward_day = forward_day;
	getline(cin, te);
}

void GlobalMessage::Set() {
	//读
	string te = "\n";
	int All_Require;
	cin >> All_Require;
	getline(cin, te);
	vector<LogicalOperation> op_list;
	for (int j = 0; j < All_Require; j++) {
		char szBuf[256];
		cin.getline(szBuf, 256);
		string temp(szBuf);
		temp = temp.substr(temp.find("(") + 1, temp.find(")") - temp.find("(") - 1);
		vector<string> strDest;
		splitString(temp, ",", strDest);
		if (strDest[0] == "add") {
			LogicalOperation add_op(strDest[0], trim(strDest[1]), trim(strDest[2]));
			op_list.push_back(add_op);
			Get().all_unadd_core += Get().xinuji_map[trim(strDest[1])].neihe;
			Get().all_unadd_memory += Get().xinuji_map[trim(strDest[1])].neicun;
		}
		else {
			LogicalOperation del_op(strDest[0], trim(strDest[1]));
			GlobalMessage::Get().all_unadd_memory -= Get().allneed_xinuji_map[trim(strDest[1])].neicun;
			GlobalMessage::Get().all_unadd_core -= Get().allneed_xinuji_map[trim(strDest[1])].neihe;
			op_list.push_back(del_op);
		}
	}
	Get().op_list_vec.push_back(op_list);
}

void GlobalMessage::Step() {
	Init();
	int All_Day = Get().all_day;
	int forward_day = Get().forward_day;
	for (int i = 0; i < All_Day; i++) {
		//记录
		if (i < forward_day) {
			Set();
		}
		if (i == forward_day) {
			CalXunijiWillContinueDay(0, forward_day - 1);
		}
		if (i >= forward_day && i < All_Day) {
			Get().dealOneday();
			Get().current_Day++;
			Set();
			CalXunijiWillContinueDay(i, i);
		}
	}
	for (int i = 0; i < forward_day; i++) {
		Get().dealOneday();
		Get().current_Day++;
	}
}


//#include<time.h>

int main()
{
	// TODO:read standard input
	//double before = clock();
	GlobalMessage& temp = GlobalMessage::Get();
	temp.Step();
	//temp.Set("training-2.txt");
	return 0;
}
