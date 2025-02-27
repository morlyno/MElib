
#define ENABLE_ASSERT 1
#define ENABLE_VERIFY 1

#include "MElib/MElib.h"

#include <vector>
#include <string>


using namespace MElib::Types;

class DataClass : public MElib::RefCounted
{
public:
	int Number = 32;
	std::vector<std::string> Strings;

	virtual const char* GetMessage() = 0;

	DataClass() = default;
	DataClass(std::string_view firstMessage, std::string_view secodnMessage)
		: Strings({ std::string(firstMessage), std::string(secodnMessage) })
	{
	}

};

class DerivedData : public DataClass
{
public:
	const char* Message = "Hallo";

	virtual const char* GetMessage() override { return Message; }

	DerivedData(std::string_view firstMessage, std::string_view secodnMessage)
		: DataClass(firstMessage, secodnMessage)
	{
	}
};

class DerivedData2 : public DataClass
{
public:
	std::string Message = "Tschüss";
	float Float = 0.2f;

	virtual const char* GetMessage() override { return Message.c_str(); }

	DerivedData2() = default;
};

MElib::Ref<DataClass> CreateData(std::string_view firstMessage, std::string_view secodnMessage)
{
	return MElib::Ref<DerivedData>::Create(firstMessage, secodnMessage);
}

int main()
{
	MElib::Logging::SetLogLevelTrace();

	MElib::Logging::Info("Hallo!");

	auto data = CreateData("Test0", "asdjlkghs");
	MElib::Logging::Trace(data->GetMessage());

	auto derived = data.As<DerivedData>();
	MElib::Logging::Debug(derived->Message);

	MElib::Ref<DerivedData2> derived2 = data.AsSave<DerivedData2>();

	MElib::Logging::Warn(std::format("Result was {}", (void*)derived2.Raw()));



	using MElib::Scope;
	Scope<DataClass> scopedData = Scope<DerivedData2>::Create();

	auto dc = scopedData.ViewAs<DerivedData2>();
	MElib::Logging::Critical(std::format("{}", dc->Float));
	MElib::Logging::Error(scopedData->GetMessage());

	using MElib::Weak;
	using MElib::Ref;

	Weak<DataClass> weak;
	{
		auto data2 = CreateData("Test0", "asdjlkghs");
		weak = data2;
		MElib::Logging::Trace(std::format("Is Alive:{}", weak.Alive()));
	}
	MElib::Logging::Trace(std::format("Is Alive:{}", weak.Alive()));


}
