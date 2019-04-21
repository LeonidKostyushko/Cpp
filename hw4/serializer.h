#include <iostream>
#include <string>

enum class Error
{
NoError,
CorruptedArchive
};

class Serializer
{
std::ostream& out;
static constexpr char Separator = ' ';
public:
explicit Serializer(std::ostream& out_)
	: out(out_)
{
}

template <class T>
Error save(T& object)
{
	return object.serialize(*this);
}

template <class... ArgsT>
Error operator()(ArgsT&&... args)
{
	return process(std::forward<ArgsT>(args)...);
}

private:
template <class T, class... ArgsT>
Error process(T&& value, ArgsT&&... args)
{
	save(std::forward<T>(value));
	out << Separator;

	return process(std::forward<ArgsT>(args)...);
}

template <class T>
Error process(T value)
{
	save(value);
	return Error::NoError;
}

void save(bool value) 
{
	out << (value ? "true" : "false") << Separator;
}

void save(uint64_t value) 
{
	out << value << Separator;
}
};

class Deserializer
{
std::istream& in;
static constexpr char Separator = ' ';

public:
explicit Deserializer(std::istream& in_)
	: in(in_) {}

template <class T>
Error load(T& obj) 
{
	return obj.serialize(*this);
}

template <class... ArgsT>
Error operator()(ArgsT&&... args)
{
	return process(std::forward<ArgsT>(args)...);
}

private:
template <class T,class...ArgsT>
Error process(T&& value, ArgsT&&	...args)
{
	if (load(std::forward<T>(value)) == false) return Error::CorruptedArchive;

	return process(std::forward<ArgsT>(args)...);
}

template <class T>
Error process(T& value)
{
	if (load(value) == false)  return Error::CorruptedArchive;

	return Error::NoError;
}

bool load(bool& value)
{
	std::string line;
	in >> line;
	if (line == "true")
	{
		value = true;
		return true;
	}
	else if (line == "false")
	{
		value = false;
		return true;
	}

	return false;
}
bool load(uint64_t& value) 
{
	std::string line;
	in >> line;
	try 
	{
		if (line[0] == '-') return false;
		value = std::stoull(line);
	}
	catch (std::invalid_argument)
	{
		return false;
	}
	return true;
}
};