#include <UnitTest++/UnitTest++.h>
#include "interface.h"
#include "auth.h"
#include "server_error.h"
#include "calc.h"

struct Interface_fixture {
    Interface * i;
    Interface_fixture()
    {
        i = new Interface();
    }
    ~Interface_fixture()
    {
        delete i;
    }
};
struct Auth_fixture {
    Auth * a;
    Auth_fixture()
    {
        a = new Auth();
    }
    ~Auth_fixture()
    {
        delete a;
    }
};
struct Calc_fixture {
    Calculation * c;
    Calc_fixture()
    {
        c = new Calculation();
    }
    ~Calc_fixture()
    {
        delete c;
    }
};
//-----------------------------------------------------------------------------
//Тесты блока интерфейса
//-----------------------------------------------------------------------------
SUITE(SettingOptions)
{
    TEST_FIXTURE(Interface_fixture, ValidOptions) {
   		Error e;
    	std::string str1 ="./program.cpp", str2="-p", str3="5555";
    	char *cstr1 = new char[str1.length() + 1];
    	char *cstr2 = new char[str2.length() + 1];
    	char *cstr3 = new char[str3.length() + 1];
		strcpy(cstr1, str1.c_str());
		strcpy(cstr2, str2.c_str());
		strcpy(cstr3, str3.c_str());
        char* arr[] = {cstr1, cstr2, cstr3};
        
        i->set_options(3, arr, e);
        CHECK_EQUAL(5555, i->get_port());
    }
    TEST_FIXTURE(Interface_fixture, WrongOptions) {
   		Error e;
    	std::string str1 ="./program.cpp", str2="-o", str3="5555";
    	char *cstr1 = new char[str1.length() + 1];
    	char *cstr2 = new char[str2.length() + 1];
    	char *cstr3 = new char[str3.length() + 1];
		strcpy(cstr1, str1.c_str());
		strcpy(cstr2, str2.c_str());
		strcpy(cstr3, str3.c_str());
        char* arr[] = {cstr1, cstr2, cstr3};
        CHECK_THROW(i->set_options(3, arr, e), boost::wrapexcept<boost::program_options::unknown_option>);
        TEST_FIXTURE(Interface_fixture, OptionWithoutValue) {
		Error e;
    	std::string str1 ="./program.cpp", str2="-p";
    	char *cstr1 = new char[str1.length() + 1];
    	char *cstr2 = new char[str2.length() + 1];
		strcpy(cstr1, str1.c_str());
		strcpy(cstr2, str2.c_str());
        char* arr[] = {cstr1, cstr2};
        CHECK_THROW(i->set_options(2, arr, e), boost::wrapexcept<boost::program_options::invalid_command_line_syntax>);
	}
}
	TEST_FIXTURE(Interface_fixture, TooBigPort) {
		Error e;
    	std::string str1 ="./program.cpp", str2="-p", str3="71783";
    	char *cstr1 = new char[str1.length() + 1];
    	char *cstr2 = new char[str2.length() + 1];
    	char *cstr3 = new char[str3.length() + 1];
		strcpy(cstr1, str1.c_str());
		strcpy(cstr2, str2.c_str());
		strcpy(cstr3, str3.c_str());
        char* arr[] = {cstr1, cstr2, cstr3};
        CHECK_THROW(i->set_options(3, arr, e), option_error);
}
	TEST_FIXTURE(Interface_fixture, TooSmallPort) {
		Error e;
    	std::string str1 ="./program.cpp", str2="-p", str3="748";
    	char *cstr1 = new char[str1.length() + 1];
    	char *cstr2 = new char[str2.length() + 1];
    	char *cstr3 = new char[str3.length() + 1];
		strcpy(cstr1, str1.c_str());
		strcpy(cstr2, str2.c_str());
		strcpy(cstr3, str3.c_str());
        char* arr[] = {cstr1, cstr2, cstr3};
        CHECK_THROW(i->set_options(3, arr, e), option_error);
	}
}
//-----------------------------------------------------------------------------
//Тесты блока аутентификации
//-----------------------------------------------------------------------------
SUITE(IdChecking)
{
    TEST_FIXTURE(Auth_fixture, ValidId) {
        std::map<std::string, std::string> base_c=a->base_read("base.txt");
        CHECK(a->id_check("user",base_c));
    }
    TEST_FIXTURE(Auth_fixture, WrongId) {
        std::map<std::string, std::string> base_c=a->base_read("base.txt");
        CHECK_THROW(a->id_check("Neizveztniy",base_c), auth_error);
    }
}
SUITE(PasswordHashing)
{
    TEST_FIXTURE(Auth_fixture, UserPassword) {
        std::map<std::string, std::string> base_c=a->base_read("base.txt");
        std::string salt=a->salt_gen();
        std::string pw1=a->pw_hash(salt, "P@ssW0rd");
        std::string pw2=a->pw_hash(salt, base_c["user"]);
        CHECK(pw_check(pw1,pw2));
    }
}
SUITE(PasswordChecking){
	TEST_FIXTURE(Auth_fixture, ValidPassword) {
        std::string salt=a->salt_gen();
        std::string pw1=a->pw_hash(salt, "P@ssW0rd");
        std::string pw2=a->pw_hash(salt, "P@ssW0rd");
        CHECK(a->pw_check(pw1,pw2));
    }
    TEST_FIXTURE(Auth_fixture, WrongPassword) {
        std::string salt=a->salt_gen();
        std::string pw1=a->pw_hash(salt, "P@ssW0rd");
        std::string pw2=a->pw_hash(salt, "P@ssW0rd56287");
        CHECK_THROW(a->pw_check(pw1,pw2), auth_error);
    }
}
SUITE(BaseReading)
{
    TEST_FIXTURE(Auth_fixture, EmptyBase) {
        CHECK_THROW(a->base_read("emptyfile.txt"), std::length_error);
    }
    TEST_FIXTURE(Auth_fixture, NotExistentBase) {
        CHECK_THROW(a->base_read("NotExistingBase.txt"), std::system_error);
    }
    TEST_FIXTURE(Auth_fixture, RegularWork) {
        std::map<std::string, std::string> base, base_c;
        base["fewh"]="464354";
        base["gf"]="ewuhwf";
        base["user"]="P@ssW0rd";
        base["Someone"]="12fjrcb%72"; 
        base_c=a->base_read("base.txt");
        CHECK(base["fewh"] == base_c["fewh"] && base["gf"] == base_c["gf"] &&
        base["user"] == base_c["user"] && base["Someone"] == base_c["Someone"]);
    }
}
//-----------------------------------------------------------------------------
//Тесты блока обработки исключений
//-----------------------------------------------------------------------------
SUITE(LogWriting){
    TEST(EmptyLogFileName){
    	Error e;
        CHECK_THROW(e.write_log("", "Error_string"), log_error);
    }
    TEST(RegularWork){
    	Error e;
        std::time_t now = time(0);
        std::string date=std::ctime(&now);
		date.erase(std::remove(date.begin(), date.end(), '\n'), date.cend());
        std::string buf, log_str;
        log_str = date+" - Error_string - Not Critical error";
        e.write_log("log.txt", "Error_string");
        std::ifstream file("log.txt");
        getline(file, buf);
        CHECK_EQUAL(log_str, buf);
    }
}
//-----------------------------------------------------------------------------
//Тесты блока вычислений
//-----------------------------------------------------------------------------

SUITE(Overflow)
{
    TEST(BelowBorder) {
        float dg=15*pow(10,20);
        float res= dg*dg;
        Calculation c; 
        CHECK_EQUAL(-FLT_MAX, c.overflow(-res));
    }
    TEST(PositiveNormalGigit) {
        float dg=15*pow(10,2);
        float res= dg*dg;
        Calculation c;
        CHECK_CLOSE(225.0f*pow(10, 4), c.overflow(res), 0.00000000001);
    }
     TEST(NegativeNormalGigit) {
        float dg=135.54*pow(10,2);
        float res= -dg*dg;
        Calculation c;
        CHECK_CLOSE(-183710916, c.overflow(res), 1);
    }
    TEST(UponBorder) {
        float dg=15*pow(10,20);
        float res= dg*dg;
        Calculation c;
        CHECK_EQUAL(FLT_MAX, c.overflow(res));
    }
    
}
SUITE(Calculation)
{
    TEST_FIXTURE(Calc_fixture, EmptyVector) {
        std::vector<float> vct = {};
        CHECK_CLOSE(1, c->multip(vct), 0.00000000001);
    }
    TEST_FIXTURE(Calc_fixture, BigVector) {
        std::vector<float> vct = {3, 15, 3, 8.3, 78, 63.6, 52, 794.43, 894.125};
        CHECK_CLOSE(205315055550346.014f, c->multip(vct), pow(10,8));
    }
    TEST_FIXTURE(Calc_fixture, PositiveNumbers) {
        std::vector<float> vct = {1.5, 4, 15};
        CHECK_CLOSE(90.0f, c->multip(vct), 0.00000000001);
    }
    TEST_FIXTURE(Calc_fixture, EvenNegativeNumbers) {
        std::vector<float> vct = {-1.5, -4, -15, -2.5};
        CHECK_CLOSE(225.0f, c->multip(vct), 0.00000000001);
    }
    TEST_FIXTURE(Calc_fixture, OddNegativeNumbers) {
        std::vector<float> vct = {-1.5, -4, -15, 2.5};
        CHECK_CLOSE(-225.0f, c->multip(vct), 0.00000000001);
    }
    TEST_FIXTURE(Calc_fixture, VectorWithZero) {
        std::vector<float> vct = {-1.5, -4, -15, 2.5, 673.59, 239.94, 932, 0, 0.821};
        CHECK_CLOSE(0, c->multip(vct), 0.00000000001);
    }
    TEST_FIXTURE(Calc_fixture, OverflowUp) {
        float num1 = pow(15.4536, 15);
        float num2 = pow(26.89034, 17);
        std::vector<float> vct = {num1, num2};
        CHECK_CLOSE(INFINITY, c->multip(vct), 0.00000000001);
    }
    TEST_FIXTURE(Calc_fixture, OverflowDown) {
        float num1 = pow(15.4536, 16);
        float num2 = pow(-26.89034, 17);
        std::vector<float> vct = {num1, num2};
        CHECK_CLOSE(-INFINITY, c->multip(vct), 0.00000000001);
    }
}
int main(int argc, char **argv)
{
    return UnitTest::RunAllTests();
}
