#include <windows.h> // За управление на цвета и забавянето
#include <thread>    // За std::this_thread::sleep_for
#include <chrono>   
#include <iostream>
#include <string>
#include <versionhelpers.h> // За проверка на версията на Windows
#include <fstream>


using namespace std;

// Функция за задаване на цвят в конзолата
void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Loading() {
    string text = "                                             M A D E   B Y   F A K L A T A                                             ";
    int delayMs = 5; // Забавяне между символите
    int delayOut = 1000;
    int colors[] = { 5 }; // Преливащи цветове
    int colorCount = sizeof(colors) / sizeof(colors[0]);
    int currentColorIndex = 0; // За началния цвят

    for (size_t i = 0; i < text.length(); ++i) {
        if (text[i] != ' ') { // Пропускаме празните интервали
            setConsoleColor(colors[currentColorIndex]); // Сменяме цвета
            currentColorIndex = (currentColorIndex + 1) % colorCount; // Преминаваме към следващия цвят
        }
        else {
            setConsoleColor(2); // Бял цвят за интервалите
        }
        cout << text[i];
        this_thread::sleep_for(chrono::milliseconds(delayMs)); // Забавяне
    }

    // Връщаме стандартния цвят
    setConsoleColor(15);
    cout << endl;
    this_thread::sleep_for(chrono::milliseconds(delayOut)); // Забавяне
    system("cls");
}

string getUserName() {
    char userName[256];
    DWORD size = sizeof(userName);
    if (GetUserNameA(userName, &size)) {
        return string(userName);
    }
    return "Unknown";
}

// Функция за получаване на името на компютъра
string getComputerName() {
    char computerName[256];
    DWORD size = sizeof(computerName);
    if (GetComputerNameA(computerName, &size)) {
        return string(computerName);
    }
    return "Unknown";
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
}

string getOperatingSystem() {
    string result = "";
    // Изпълняваме командата и пренасочваме изхода в текстов файл
    system("systeminfo | findstr /B /C:\"OS Name\" > os_info.txt");

    // Четем резултата от файла
    ifstream file("os_info.txt");
    if (file.is_open()) {
        getline(file, result); // Четем първия ред
        file.close();
    }
    else {
        cerr << "Unknow OS!?!?" << endl;
    }

    // Премахваме "OS Name: " и водещите интервали
    size_t pos = result.find("OS Name:");
    if (pos != string::npos) {
        result = result.substr(pos + 9); // Премахваме "OS Name: "
    }

    // Изчистваме водещите интервали
    result = trim(result);

    return result;
}

string getTimeZone() {
    TIME_ZONE_INFORMATION tzInfo;
    DWORD result = GetTimeZoneInformation(&tzInfo);  // Получаваме информация за времевата зона

    if (result == TIME_ZONE_ID_INVALID) {
        return "Unknown";  // Не може да се получи информация
    }

    string timeZone = "UTC";
    timeZone += (tzInfo.Bias < 0 ? "+" : "-") + to_string(abs(tzInfo.Bias) / 60) + ":" + to_string(abs(tzInfo.Bias) % 60);

    // Преобразуване на StandardName (wstring) в string
    wstring wstr(tzInfo.StandardName);  // Създаване на wstring от wide string
    string standardName(wstr.begin(), wstr.end());  // Конвертиране на wstring към string
    timeZone += " | " + standardName;  // Добавяме стандартното име на времевата зона

    return timeZone;
}

void MainMenuInformation()
{
    // Текстоове
    string text1 = "=====================================================================================================================\n";  // Първи ред (без цвят)
    string text2 = "USER:  | COMPUTERNAME: ";  // Лейбъл за USER и COMPUTERNAME
    string text3 = "CURRENT OS: ";  // Лейбъл за операционната система
    string text4 = "TIME ZONE: ";  // Лейбъл за времевата зона
    string text5 = "=====================================================================================================================\n";  // Последен ред (без цвят)

    // Забавяне между символите
    int delayMs = 250;  // Забавяне между символите

    // Цветове
    int valueColor = 4;  // За етикетите (бял)
    int labelColor = 15;  // За стойностите (жълт)

    // Получаваме информацията
    string userName = getUserName();
    string computerName = getComputerName();
    string operatingSystem = getOperatingSystem();
    string timeZone = getTimeZone();

    // Масив с текстове, които ще изписваме ред по ред
    string texts[] = {
        text1,  // Първи ред без цвят
        "USER: ",  // Лейбъл без цвят
        userName,  // Стойност с жълт цвят
        " | COMPUTERNAME: ",  // Лейбъл без цвят
        computerName,  // Стойност с жълт цвят
        "\nCURRENT OS: ",  // Лейбъл без цвят
        operatingSystem,  // Стойност с жълт цвят
        "\nTIME ZONE: ",  // Лейбъл без цвят
        timeZone, // Стойност с жълт цвят
        "\n",  
        text5  // Последен ред без цвят
    };

    // Обработваме всеки ред поотделно
    for (int j = 0; j < 11; ++j) {
        string currentText = texts[j];

        // За всеки ред от текста
        for (size_t i = 0; i < currentText.length(); ++i) {
            if (currentText[i] != ' ') { // Пропускаме празните интервали
                if (j == 0 || j == 1 || j == 3 || j == 7 || j == 5 || j == 10) {
                    // Лейбълите (USER:, COMPUTERNAME:, CURRENT OS:, TIME ZONE:) са без цвят
                    setConsoleColor(labelColor);  // Бял цвят за етикетите
                }
                else {
                    // Стойностите на променливите са в жълт цвят
                    setConsoleColor(valueColor);  // Жълт цвят за стойностите
                }
            }
            else {
                setConsoleColor(15); // Бял цвят за интервалите
            }
            cout << currentText[i];
            this_thread::sleep_for(chrono::nanoseconds(delayMs)); // Забавяне между символите
        }
    }

    // Връщаме стандартния цвят
    setConsoleColor(15);
}

void CleanTemporaryFiles()
{
    cout << "=====================================================================================================================\n";
    system("del /q /f /s C:\\Windows\\Temp\\*");
    system("del /q /f /s C:\\Windows\\Prefetch\\*");
    system("del /q /f /s \"C:\\Users\\%username%\\AppData\\Local\\Temp\\*\"");
    system("pause");
    system("cls");
}

//AW14
void ActivateWindows()
{
    string operatingSystem = getOperatingSystem();
    string operatingVersion;
    string command = "slmgr /ipk " + operatingVersion;
    if (operatingSystem == "Microsoft Windows 10 Home")
    {
        operatingVersion = "TX9XD-98N7V-6WMQ6-BX7FG-H8Q99";
    }
    else if(operatingSystem == "Microsoft Windows 10 Home N")
    {
        operatingVersion = "3KHY7-WNT83-DGQKR-F7HPR-844BM";
    }
    else if(operatingSystem == "Microsoft Windows 10 Home Single Language")
    {
        operatingVersion = "7HNRX-D7KGG-3K4RQ-4WPJ4-YTDFH";
    }
    else if(operatingSystem == "Microsoft Windows 10 Home Country Specific")
    {
        operatingVersion = "PVMJN-6DFY6-9CCP6-7BKTT-D3WVR";
    }
    else if(operatingSystem == "Microsoft Windows 10 Professional" || "Microsoft Windows 10 Pro" )
    {
        operatingVersion = "W269N-WFGWX-YVC9B-4J6C9-T83GX";
    }
    else if(operatingSystem == "Microsoft Windows 10 Professional N" || "Microsoft Windows 10 Pro N")
    {
        operatingVersion = "MH37W-N47XK-V7XM9-C7227-GCQG9";
    }
    else if(operatingSystem == "Microsoft Windows 10 Education")
    {
        operatingVersion = "NW6C2-QMPVW-D7KKK-3GKT6-VCFB2";
    }
    else if(operatingSystem == "Microsoft Windows 10 Education N")
    {
        operatingVersion = "2WH4N-8QGBV-H22JP-CT43Q-MDWWJ";
    }
    else if(operatingSystem == "Microsoft Windows 10 Enterprise")
    {
        operatingVersion = "NPPR9-FWDCX-D2C8J-H872K-2YT43";
    }
    else if(operatingSystem == "Microsoft Windows 10 Enterprise N")
    {
        operatingVersion = "DPH2V-TTNVB-4X9Q3-TJR4H-KHJW4";
    }
    else {
        cout << "Error AW14: Contact with FaklaTa!";
    }

    system(command.c_str());
    system("slmgr /skms kms8.msguides.com");
    system("slmgr /ato");

    


        

}

void ActivationSoftwareMenu()
{
    unsigned short int MenuChoice;

    do {
        cout << "=====================================================================================================================\n";
        cout << "       Activation software!\n";
        cout << "=====================================================================================================================\n";
        cout << "       [1] Activate Windows \033[31m!!!YOU NEED NETWORK CONNECTION!!!\033[0m\n";
        cout << "       [2] Activate OfficePacket (COMING SOON)\n";
        cout << "       [0] Back\n";
        cout << "=====================================================================================================================\n";
        cout << "       Enter number: ";

        cin >> MenuChoice;

        switch (MenuChoice) {
        case 1:
        {
            cout << "\033[31mClick OK on all windows that appear.\033[0m" << endl;
            cout << "\033[33mPlease Wait...\033[0m" << endl;
            ActivateWindows();
            cout << "\033[32mWindows is ACTIVATED\033[0m" << endl;
            break;
        }
        case 0:
        {
            return;
        }
        default:
        {
            cout << "Invalid number!";
            break;
        }
        }
        system("pause");
        system("cls");

    } while (true);


}

void InternetSoftwareMenu()
{
    unsigned short int MenuChoice;
    do {
        cout << "=====================================================================================================================\n";
        cout << system("ping google.com -n 1 > nul 2>&1 && echo \033[32mYou are connected to the Internet\033[0m || echo \033[31mYou are not connected to the Internet\033[0m ");
        cout << "=====================================================================================================================\n";
        cout << "       [1] Ping Test \033[36m(google.com)\033[0m\n";
        cout << "       [2] Internet SpeedTest\n";
        cout << "       [3] Show Open Ports\n";
        cout << "       [0] Back\n";
        cout << "=====================================================================================================================\n";
        cout << "       Enter number: ";

        cin >> MenuChoice;

        switch (MenuChoice) {

            case 1:
                system("ping google.com");
                break;
            case 2:
                system("speedtest.exe");
                break;
            case 3:
                system("netstat -ano");
                break;
            case 0:
                
                return;
            default:
                cout << "Invalid number!";
                break;

        }
        system("pause");
        system("cls");

    } while (true);


}

void CleaningSoftware()
{
    unsigned short int MenuChoice;

    do {
        cout << "=====================================================================================================================\n";
        cout << "       Cleaning Software\n";
        cout << "=====================================================================================================================\n";
        cout << "       [1] Clean temporary files\n";
        cout << "       [2] Deep Delete application (COMMING SOON)\n";
        cout << "       [0] Back\n";
        cout << "=====================================================================================================================\n";
        cout << "       Enter number: ";

        cin >> MenuChoice;

        switch (MenuChoice) {
        case 1:
        {
            CleanTemporaryFiles();
            break;
        }
        case 0:
        {
            return;
        }
        default:
        {
            cout << "Invalid number!";
            break;
        }
        }
        system("pause");
        system("cls");

    } while (true);


}

void OptimizationForGaming()
{
    cout << "=====================================================================================================================\n";
    const char* command1 = R"(reg add "HKEY_CURRENT_USER\Software\Microsoft\GameBar" /v "AutoGameModeEnabled" /t REG_DWORD /d 1 /f")";
    system(command1);
    const char* command2 = R"(reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\BackgroundAccessApplications" /v "GlobalUserDisabled" /t REG_DWORD /d 1 /f)";
    system(command2);
    const char* command3 = R"(reg add "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\GameDVR" /v "AppCaptureEnabled" /t REG_DWORD /d 0 /f)";
    system(command3);
    const char* command4 = R"(reg add "HKEY_CURRENT_USER\Software\Microsoft\GameBar" /v "ShowStartupPanel" /t REG_DWORD /d 0 /f)";
    system(command4);
    
    system("netsh interface tcp set global autotuninglevel=disabled");
    system("powercfg /setactive SCHEME_MIN");
    system("netsh interface tcp set global autotuninglevel=disabled");

    system("netsh interface tcp set global autotuninglevel=disabled");
    system("start ms-settings:display-advancedgraphics");
   
    MessageBox(NULL,
        L"След като прозореца се е отворил, следвайте тези стъпки:\n1. Активирайте Hardware-accelerated GPU Scheduling.\n2. Изберете приложения през browse, и ги поставете на High Performance.\n3. Завършете и затворете прозореца. 4. Част от найстойките ще влезнат в сила след рестартиране на компютъра.\n",
        L"Optimization for Gaming",
        MB_OK | MB_ICONINFORMATION);
    
    
    cout << "\033[33mOptimizated for GAMING\033[0m\n";
    cout << "\033[31m!!!The computer needs to be restarted for the changes to take effect!!!\033[0m\n";

}

void InformationForOptimizationForGaming()
{
    cout << "=====================================================================================================================\n";
    cout << "Information for Optimization for GAMING\n";
    cout << "=====================================================================================================================\n";
    cout << "The Optimization for Gaming option applies settings in Windows to enhance performance during gaming by:\n";
    cout << "1. Enabling Automatic Game Mode – Prioritizes game processes to improve performance by reducing the resources allocated to other applications.\n";
    cout << "2. Disabling Background App Activity – Minimizes system load by preventing background applications from consuming resources.\n";
    cout << "3. Turning Off Game DVR (Screen Recording) – Disables automatic recording and capturing features to reduce CPU and disk usage.\n";
    cout << "4. Hiding the Game Bar Startup Panel – Eliminates visual distractions and reduces the resources used by the Game Bar.\n";
    cout << "5. Optimizing Network Settings – Disables TCP auto-tuning features that may cause network latency.\n";
    cout << "6. Activating High Performance Power Mode – Switches the power plan to 'High Performance' for maximum hardware output.\n";
    cout << "=====================================================================================================================\n";
}
void OptimizationSoftware()
{
    unsigned short int MenuChoice;

    do {
        cout << "=====================================================================================================================\n";
        cout << "       Cleaning Software \033[31m!!!If you don't understand, do not touch this!!!\033[0m\n";
        cout << "=====================================================================================================================\n";
        cout << "       [1] Optimization for GAMING\n";
        cout << "       [2] Optimization for Work\n";
        cout << "       [3] Information\n";
        cout << "       [0] Back\n";
        cout << "=====================================================================================================================\n";
        cout << "       Enter number: ";

        cin >> MenuChoice;

        switch (MenuChoice) {
        case 1:
        {
            OptimizationForGaming();
            break;
        }
        case 3:
            InformationForOptimizationForGaming();
            break;
        case 0:
        {
            return;
        }
        default:
        {
            cout << "Invalid number!";
            break;
        }
        }
        system("pause");
        system("cls");

    } while (true);
}





int main()
{
    unsigned short int MenuChoice;

    Loading();
    
    do {
        
        MainMenuInformation();
        cout << "       [1] Activation software\n";
        cout << "       [2] Internet software\n";
        cout << "       [3] Download software\n";
        cout << "       [4] Optimization software\n";
        cout << "       [5] Cleaning software\n";
        cout << "       [0] Exit\n";
        cout << "=====================================================================================================================\n";
        cout << "       Enter number: ";

        cin >> MenuChoice;

        switch (MenuChoice) {
        case 1:
            system("cls");
            ActivationSoftwareMenu();
            break;

        case 2:
            system("cls");
            InternetSoftwareMenu();
            break;

        case 4:
            system("cls");
            OptimizationSoftware();
            break;

        case 5:
            system("cls");
            CleaningSoftware();
            break;

        default:
            cout << "Invalid number!";
        }
        system("pause");
        system("cls");

    } while (true);
    
    return 0;
}
