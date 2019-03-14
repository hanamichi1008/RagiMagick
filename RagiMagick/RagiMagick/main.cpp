#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "ragii/image/Bitmap.h"
#include "ragii/image/BitmapConverter.h"
#include "ragii/text/text.h"


#ifdef _MSC_VER // MSVC
#define ASM_BEGIN() __asm {
#define ASM_DIRECTIVE(D)
#define ASM_OPERAND0(OPCODE) OPCODE
#define ASM_OPERAND1(OPCODE, OPERAND) OPCODE OPERAND
#define ASM_OPERAND2(OPCODE, OPERAND1, OPERAND2) OPCODE OPERAND1, OPERAND2
#define ASM_OUTPUT_BEGIN()
#define ASM_OUTPUT_VALUE(MODE, VARIABLE)
#define ASM_OUTPUT_END()
#define ASM_END() }
#else // clang or gcc
#define ASM_BEGIN() asm volatile (
#define ASM_TEXT(T) #T
#define ASM_TEXT_LF(T) #T " \n"
#define ASM_COMMA() ,
#define ASM_DIRECTIVE(D) ASM_TEXT_LF(D)
#define ASM_OPERAND0(OPCODE) ASM_TEXT_LF(OPCODE)
#define ASM_OPERAND1(OPCODE, OPERAND) OPCODE OPERAND " \n"
#define ASM_OPERAND2_IMPL(OPCODE, OPERANDS)  #OPCODE " " OPERANDS " \n"
#define ASM_OPERAND2(OPCODE, ...) ASM_OPERAND2_IMPL(OPCODE, #__VA_ARGS__)
#define ASM_OUTPUT_BEGIN() :
#define ASM_OUTPUT_VALUE(MODE, VARIABLE) #MODE(VARIABLE)
#define ASM_OUTPUT_END()
#define ASM_END() );
#endif

using namespace std;
using namespace ragii::image;
using namespace ragii::text;

struct CommandOption
{
	string_view name;
	string_view value;
};

void dumpSystemInfo();
int process(int argc, char* argv[]);
int convert(vector<CommandOption>& opts);
int create(vector<CommandOption>& opts);

int main(int argc, char* argv[])
{
	dumpSystemInfo();

	return process(argc, argv);
}

void dumpSystemInfo()
{
	uint32_t a, b, c, d = 0;
	//asm volatile (
	//	".intel_syntax noprefix \n"
	//	"xor %%eax, %%eax \n"
	//	"cpuid \n"
	//	: "=a" (a), "=b" (b), "=c" (c), "=d" (d)
	//);

	// 実験的にやってみた。やっぱり消す。共通化は無駄な労力。
	ASM_BEGIN()
		ASM_DIRECTIVE(.intel_syntax noprefix) // gcc だとコンパイルエラー
		ASM_OPERAND2(xor, eax, eax)
		ASM_OPERAND0(cpuid)
		ASM_OUTPUT_BEGIN()
			ASM_OUTPUT_VALUE(=a, a)
		ASM_OUTPUT_END()
#ifdef _MSC_VER
		ASM_OPERAND2(mov, a, eax)
#endif
	ASM_END()

	printf("===========================\n");
	char values[4 + 1] = {};
	arithmetic_to_str<char, uint32_t>(b, values);
	printf("ebx: %s (0x%x)\n", values, b);
	arithmetic_to_str<char, uint32_t>(d, values);
	printf("edx: %s (0x%x)\n", values, d);
	arithmetic_to_str<char, uint32_t>(c, values);
	printf("ecx: %s (0x%x)\n", values, c);
	printf("- - - - - - - - - - - - - -\n");
	printf("eax: 0x%x\n", a);
	printf("ebx: 0x%x\n", b);
	printf("ecx: 0x%x\n", c);
	printf("edx: 0x%x\n", d);
	printf("===========================\n");
}

int process(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "コマンドを指定してください。 convert, etc." << endl;
		return EXIT_FAILURE;
	}

	vector<CommandOption> opts;

	for (int i = 1; i < argc; )
	{
		auto opt = string_view(argv[i]);
		if (opt[0] == '-')
		{
			auto value = i + 1 < argc ? argv[i + 1] : "";
			opts.emplace_back(CommandOption { argv[i], value });
			i += 2;
		}
		else
		{
			opts.emplace_back(CommandOption { argv[i], "" });
			i++;
		}
	}

	for (auto i : opts)
	{
		cout << i.name.data() << ": " << i.value.data() << endl;
	}

	if (opts.empty())
	{
		cout << "コマンドを指定してください。 convert, etc." << endl;
		return EXIT_FAILURE;
	}

	auto command = opts[0].name;
	opts.erase(opts.begin());

	if (command == "convert")
	{
		return convert(opts);
	}
	else if (command == "create")
	{
		return create(opts);
	}
	else
	{
		cout << "コマンドを指定してください。 convert, etc." << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int convert(vector<CommandOption>& opts)
{
	if (opts.empty())
	{
		cout << "変換方法を指定してください。 negative, grayscale, etc." << endl;
		return EXIT_FAILURE;
	}

	auto in_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-i"; });
	if (in_file == opts.end())
	{
		cout << "入力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (in_file->value.empty())
	{
		cout << "入力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto out_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-o"; });
	if (out_file == opts.end())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (out_file->value.empty())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	if (!ends_with(in_file->value.data(), ".bmp"))
	{
		cout << ".bmp 以外は非対応です。" << endl;
		return EXIT_FAILURE;
	}

	auto bmp = Bitmap::loadFromFile(in_file->value.data());

	if (!bmp)
	{
		cout << "ファイルのロードに失敗しました。" << endl;
		return EXIT_FAILURE;
	}

	cout << "width: " << bmp->getWidth() << ", height: " << bmp->getHeight() << ", depth: " << bmp->getBitCount() / 8 << endl;

	auto filter = opts[0].name;

	if (filter == "negative")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Negative);
	}
	else if (filter == "binary")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Binary);
	}
	else if (filter == "grayscale")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Grayscale);
	}
	else if (filter == "laplacian")
	{
		BitmapConverter::applyFilter(bmp.get(), FilterType::Grayscale);
		BitmapConverter::applyFilter(bmp.get(), FilterType::Laplacian);
	}

	bmp->save(out_file->value.data());
	cout << "converted." << endl;

	return EXIT_SUCCESS;
}

// RagiMagick create -w 32 -h 32 -d 3 -p checkered -o out.bmp
int create(vector<CommandOption>& opts)
{
	if (opts.empty())
	{
		cout << "オプションが不足しています。" << endl;
		return EXIT_FAILURE;
	}

	auto out_file = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-o"; });
	if (out_file == opts.end())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (out_file->value.empty())
	{
		cout << "出力ファイル名を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto w = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-w"; });
	if (w == opts.end())
	{
		cout << "-w (幅) を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (w->value.empty())
	{
		cout << "幅の値を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto h = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-h"; });
	if (h == opts.end())
	{
		cout << "-h (高さ) を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (h->value.empty())
	{
		cout << "高さの値を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto d = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-d"; });
	if (d == opts.end())
	{
		cout << "-d (ビット深度 3: 24bit, 4: 32bit) を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (d->value.empty())
	{
		cout << "ビット深度の値を指定してください。" << endl;
		return EXIT_FAILURE;
	}
	if (!is_digit(d->value.data()[0]))
	{
		cout << "ビット深度の値が不正です。" << endl;
		return EXIT_FAILURE;
	}

	auto p = find_if(opts.begin(), opts.end(), [&](auto i) { return i.name == "-p"; });
	if (p == opts.end())
	{
		cout << "-p (パターン) を指定してください。" << endl;
		return EXIT_FAILURE;
	}

	auto bmp = 
		Bitmap::create(
			str_to_arithmetic<int32_t>(w->value.data()),
			str_to_arithmetic<int32_t>(h->value.data()),
			str_to_arithmetic<int16_t>(d->value.data()) * 8
		);

	if (p->value == "checkered")
	{
		auto data = bmp->getData().get();
		auto depth = bmp->getBitCount() / 8;
		for (int y = 0; y < bmp->getHeight(); y++)
		{
			for (int x = 0; x < bmp->getWidth(); x++)
			{
				for (int i = 0; i < depth; i++)
				{
					*data++ = y ^ x;
				}
			}
		}
	}

	bmp->save(out_file->value.data());

	cout << "created." << endl;

	return EXIT_SUCCESS;
}
