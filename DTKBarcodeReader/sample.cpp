//
// DTK Barcode Reader C++ sample application 
// 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <ctime>
using namespace std;

#include "DTKBarReader5.h"
#pragma comment(lib, "DTKBarReader5.lib")

#ifdef _WIN32
#define CCONV __stdcall
#else
#define CCONV
#endif

string get_license_type_string(int type)
{
	switch (type)
	{
	case 1: return "Workstation 1D";
	case 2: return "Workstation 1D2D";
	case 3: return "Unlimited Workstation 1D";
	case 4: return "Unlimited Workstation 1D2D";
	case 5: return "Server 1D";
	case 6: return "Server 1D2D";
	case 7: return "Unlimited Server 1D";
	case 8: return "Unlimited Server 1D2D";
	default:
		return "Unknown";
	}
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Invalid parameters" << endl;
		cout << "Parameters:" << endl;
		cout << "\t<image file path>" << endl;
		cout << "\tlicinfo" << endl;
		cout << "\tlicactivate <license key>" << endl;
		cout << "Press enter to continue..." << endl;
		return 0;
	}
	int res;
	string param = argv[1];

	if (param.substr(0, 7) == "licinfo")
	{
		char lic_key[128];
		char comments[256];
		int licenseType;
		time_t exp_date;
		BarcodeReader_GetActivatedLicenseInfo(lic_key, 128, comments, 256, &licenseType, &exp_date);
		if (strlen(lic_key) > 0)
		{
			cout << "You have following license:" << endl;
			cout << "License Key: " << lic_key << endl;
			cout << "License Type: " << get_license_type_string(licenseType) << endl;
			cout << "Comments: " << comments << endl;
			if (exp_date > 0)
			{
				std::tm * ptm = std::localtime(&exp_date);
				char date_str[32];
				std::strftime(date_str, 32, "%Y-%m-%d", ptm);
				cout << "Exp date: " << date_str << endl;
			}
		}
		else
		{
			cout << "You have not license activated" << endl;
		}
		return 0;
	}
	if (param.substr(0, 11) == "licactivate" && argc > 2)
	{
		string lic_key = argv[2];
		res = BarcodeReader_ActivateLicenseOnline(lic_key.c_str(), "");
		if (res == 0)
			cout << "License activated successfully." << endl;
		else
			cout << "License activation failed. Error code: " << res << endl;
		return 0;
	}

	string file_path = argv[1];

	// Check license 
	char licenseKey[100];
	char comments[100];
	int licenseType;
	time_t exp_date;
	BarcodeReader_GetActivatedLicenseInfo(licenseKey, 100, comments, 100, &licenseType, &exp_date);
	if (strlen(licenseKey) == 0)
	{
		cout << "You have not valid license activated on this computer." << endl;
		return 0;
	}

	// Create BarcodeReader instance 
	BARREADER hBarReader = BarcodeReader_Create(NULL);

	// Setup barcode reader parameters
	BarcodeReader_set_BarcodeTypes(hBarReader, (BarcodeTypeEnum)(
		BarcodeTypeEnum::BT_Code39 | 
		BarcodeTypeEnum::BT_Code128 |
		BarcodeTypeEnum::BT_QRCode |
		BarcodeTypeEnum::BT_DataMatrix |
		BarcodeTypeEnum::BT_PDF417
		));
	BarcodeReader_set_Orientation(hBarReader, BarcodeOrientationEnum::BO_All);
	
	// Read barcodes from image file
	BARREADERRESULT hRes = BarcodeReader_ReadFromFile(hBarReader, file_path.c_str());
	
	int count = BarReaderResult_get_BarcodesCount(hRes);
	for (int i = 0; i < count; i++)
	{
		BARCODE hBarcode = BarReaderResult_get_Barcode(hRes, i);

		string text;
		string type;

		text.resize(Barcode_get_Text(hBarcode, NULL, 0));
		Barcode_get_Text(hBarcode, &text[0], text.size());

		type.resize(Barcode_get_TypeString(hBarcode, NULL, 0));
		Barcode_get_TypeString(hBarcode, &type[0], type.size());

		// display barcode data
		std::cout << "Barcode: " << text << " Type: " << type << endl;

		Barcode_Destroy(hBarcode);
	}

	BarReaderResult_Destroy(hRes);

	BarcodeReader_Destroy(hBarReader);
	
	return 0;
}

