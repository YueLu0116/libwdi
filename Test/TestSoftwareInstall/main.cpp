#include "../../libwdi/libwdi.h"

#include <cstdint>
#include <cstdio>
#include <string>

static char desc_[] = "Android Accessory (pre installed2)";
const uint16_t    vid_ = 0x18d1;
const uint16_t    pid_ = 0x2d00;
static char infName_[] = "aoa_test0422.inf";
static char driverDir_[] = "tmp";

HWND GetConsoleHwnd(void)
{
	HWND hwndFound;
	char pszNewWindowTitle[128];
	char pszOldWindowTitle[128];

	GetConsoleTitleA(pszOldWindowTitle, 128);
	wsprintfA(pszNewWindowTitle, "%d/%d", GetTickCount(), GetCurrentProcessId());
	SetConsoleTitleA(pszNewWindowTitle);
	Sleep(40);
	hwndFound = FindWindowA(NULL, pszNewWindowTitle);
	SetConsoleTitleA(pszOldWindowTitle);
	return hwndFound;
}

int main(int argc, char* argv[]) {
	wdi_device_info dev{ nullptr, vid_, pid_, false, 0, desc_, nullptr, nullptr, nullptr };
	wdi_device_info* pldev = &dev;
	wdi_options_create_list ocl = { 0 };
	wdi_options_prepare_driver opd = { 0 };
	wdi_options_install_driver oid = { 0 };
	wdi_options_install_cert oic = { 0 };

	static int opt_silent = 0, opt_extract = 0, log_level = WDI_LOG_LEVEL_WARNING;
	static BOOL matching_device_found;
	int r;
	char* inf_name = infName_;
	char* ext_dir = driverDir_;
	char* cert_name = nullptr;

	ocl.list_all = true;
	ocl.list_hubs = true;
	ocl.trim_whitespaces = true;
	opd.driver_type = WDI_WINUSB;
	oid.hWnd = GetConsoleHwnd();
	oic.hWnd = oid.hWnd;

	wdi_set_log_level(log_level);

	printf("Extracting driver files...\n");
	r = wdi_prepare_driver(&dev, ext_dir, inf_name, &opd);
	printf("  %s\n", wdi_strerror(r));
	if ((r != WDI_SUCCESS) || (opt_extract)) {
		system("pause");
		return r;
	}

	if (cert_name != NULL) {
		printf("Installing certificate '%s' as a Trusted Publisher...\n", cert_name);
		r = wdi_install_trusted_certificate(cert_name, &oic);
		printf("  %s\n", wdi_strerror(r));
	}

	printf("Installing driver(s)...\n");

	r = wdi_install_driver(&dev, ext_dir, inf_name, &oid);
	printf("  %s\n", wdi_strerror(r));
	system("pause");
	return r;
}