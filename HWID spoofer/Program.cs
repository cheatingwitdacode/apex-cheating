using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace HWID_spoofer
{
    class Program
    {
		// Token: 0x06000001 RID: 1 RVA: 0x00002050 File Offset: 0x00000250
		private static void Main(string[] args)
		{
			Program.r = new Random();
			Console.WriteLine("(1): Backup Keys");
			Console.WriteLine("(2): Spoof Keys");
			Console.WriteLine("(3): Restore Keys");
			Console.WriteLine("");
			Console.Write("(1):");
			ConsoleKeyInfo consoleKeyInfo = Console.ReadKey();
			Console.Clear();
			switch (consoleKeyInfo.KeyChar)
			{
				case '1':
					Program.backupKeys();
					return;
				case '2':
					Program.randomizeKeys();
					return;
				case '3':
					break;
				default:
					Program.backupKeys();
					break;
			}
		}

		// Token: 0x06000002 RID: 2 RVA: 0x000020D4 File Offset: 0x000002D4
		private static void backupKeys()
		{
			string[] array = new string[18];
			array[0] = "Windows Registry Editor Version 5.00";
			array[1] = "";
			array[2] = "[HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation]";
			object value = Microsoft.Win32.Registry.GetValue("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "SystemProductName", "");
			object value2 = Registry.GetValue("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "ComputerHardwareId", "");
			object value3 = Registry.GetValue("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "ComputerHardwareIds", "");
			array[3] = "\"SystemProductName\"=\"" + ((value != null) ? value.ToString() : null) + "\"";
			array[4] = "\"ComputerHardwareId\"=\"" + ((value2 != null) ? value2.ToString() : null) + "\"";
			array[5] = "\"ComputerHardwareIds\"=hex(7):" + Program.arrToMagic((Array)value3);
			array[6] = "";
			array[7] = "[HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0]";
			object value4 = Registry.GetValue("HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "Identifier", "");
			object value5 = Registry.GetValue("HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "SerialNumber", "");
			array[8] = "\"Identifier\"=\"" + ((value4 != null) ? value4.ToString() : null) + "\"";
			array[9] = "\"SerialNumber\"=\"" + ((value5 != null) ? value5.ToString() : null) + "\"";
			array[10] = "";
			array[11] = "[HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000]";
			object value6 = Registry.GetValue("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000", "DriverDesc", "");
			array[12] = "\"DriverDesc\"=\"" + ((value6 != null) ? value6.ToString() : null) + "\"";
			array[13] = "";
			array[14] = "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion]";
			object value7 = Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "InstallTime", 0);
			object value8 = Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "BuildGUID", "");
			object value9 = Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductID", "");
			array[15] = "\"InstallTime\"=\"" + ((value7 != null) ? value7.ToString() : null) + "\"";
			array[16] = "\"BuildGUID\"=\"" + ((value8 != null) ? value8.ToString() : null) + "\"";
			array[17] = "\"ProductID\"=\"" + ((value9 != null) ? value9.ToString() : null) + "\"";
			File.WriteAllLines("keys-" + DateTimeOffset.Now.ToUnixTimeSeconds().ToString() + ".reg", array);
			Console.WriteLine("Done! Press any key to continue.");
			Console.ReadKey();
		}

		// Token: 0x06000003 RID: 3 RVA: 0x00002350 File Offset: 0x00000550
		private static void randomizeKeys()
		{
			if (!WindowsIdentity.GetCurrent().Owner.IsWellKnown(WellKnownSidType.BuiltinAdministratorsSid))
			{
				Console.WriteLine("Please run this program as administrator.");
				Console.ReadKey();
				return;
			}
			Registry.SetValue("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\SystemInformation", "ComputerHardwareId", "{" + Guid.NewGuid().ToString() + "}");
			Registry.SetValue("HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "Identifier", Program.RandomString(15));
			Registry.SetValue("HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0", "SerialNumber", Program.RandomString(15));
			Registry.SetValue("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000", "DriverDesc", Program.gpus[Program.r.Next(0, Program.gpus.Length)]);
			Registry.SetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "BuildGUID", Guid.NewGuid());
			Registry.SetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductID", string.Concat(new string[]
			{
				Program.randomFromLength(5),
				"-",
				Program.randomFromLength(5),
				"-",
				Program.randomFromLength(5),
				"-",
				Program.randomFromLength(5)
			}));
			Console.WriteLine("Done! Press any key to continue.");
			Console.ReadKey();
		}

		// Token: 0x06000004 RID: 4 RVA: 0x00002484 File Offset: 0x00000684
		private static string randomFromLength(int length)
		{
			string text = "";
			for (int i = 0; i < length; i++)
			{
				text += Program.r.Next(0, 15).ToString("X");
			}
			return text;
		}

		// Token: 0x06000005 RID: 5 RVA: 0x000024C5 File Offset: 0x000006C5
		public static string RandomString(int length)
		{
			return new string((from s in Enumerable.Repeat<string>("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", length)
							   select s[Program.r.Next(s.Length)]).ToArray<char>());
		}

		// Token: 0x06000006 RID: 6 RVA: 0x00002500 File Offset: 0x00000700
		public static string arrToMagic(Array arr)
		{
			string str = "";
			foreach (object obj in arr)
			{
				string s = (string)obj;
				str = str + BitConverter.ToString(Encoding.Default.GetBytes(s)).Replace("-", ",00,") + ",00,00,00,";
			}
			return str + ",00,00";
		}

		// Token: 0x04000001 RID: 1
		private static Random r;

		// Token: 0x04000002 RID: 2
		private static string[] gpus = new string[]
		{
			"GeForce GTX TITAN Z",
			"NVIDIA TITAN Xp",
			"GeForce GTX 1080 Ti",
			"GeForce GTX TITAN X",
			"GeForce GTX 690",
			"GeForce GTX TITAN Black",
			"GeForce GTX 780 Ti",
			"GeForce GTX 980 Ti",
			"GeForce GTX TITAN",
			"GeForce GTX 1080",
			"GeForce GTX 780",
			"GeForce GTX 980",
			"GeForce GTX 1070",
			"GeForce GTX 970",
			"GeForce GTX 770",
			"GeForce GTX 680",
			"GeForce GTX 760 Ti (OEM)",
			"GeForce GTX 670",
			"GeForce GTX 660 Ti",
			"GeForce GTX 1060 (6GB)",
			"GeForce GTX 960 (OEM)",
			"GeForce GTX 760 192-bit(OEM)",
			"GeForce GTX 760",
			"GeForce GTX 1060 (3GB)",
			"GeForce GTX 660 (OEM)",
			"GeForce GTX 960",
			"GeForce GTX 950 (OEM)",
			"GeForce GTX 590",
			"GeForce GTX 660",
			"GeForce GTX 1050 Ti",
			"GeForce GTX 950",
			"GeForce GTX 650 Ti BOOST",
			"GeForce GTX 650 Ti",
			"GeForce GTX 1050",
			"GeForce GTX 750 Ti",
			"GeForce GTX 645 (OEM)",
			"GeForce GTX 750",
			"GeForce GTX 580",
			"GeForce GTX 480",
			"GeForce GTX 570",
			"GeForce GTX 295",
			"GeForce GTX 470",
			"GeForce GTX 745 (OEM)",
			"GeForce GT 740",
			"GeForce GT 730",
			"GeForce GT 635 (OEM)",
			"GeForce GTX 650",
			"GeForce GTX 560 Ti",
			"GeForce GTX 560 (OEM)",
			"GeForce GT 640",
			"GeForce GTX 465",
			"GeForce GTX 560 Ti (OEM)",
			"GeForce GTX 460",
			"GeForce GTX 560",
			"GeForce GTX 460 SE",
			"GeForce GTX 555 (OEM)",
			"GeForce GTX 285 for Mac",
			"GeForce GTX 285",
			"GeForce GTX 280",
			"GeForce GT 720",
			"GeForce GT 710",
			"GeForce GTS 450",
			"GeForce GTX 550 Ti",
			"GeForce GT 630 (OEM)",
			"GeForce GT 640 (OEM)",
			"GeForce GT 545 GDDR5 (OEM)",
			"GeForce GT 545 DDR3",
			"GeForce GTS 250",
			"GeForce GTS 150",
			"GeForce GTS 240 (OEM Product)",
			"GeForce GT 630",
			"GeForce GT 620",
			"GeForce GT 440",
			"GeForce GT 430",
			"GeForce GT 530 (OEM)",
			"GeForce GT 340 (OEM)",
			"GeForce GT 330 (OEM)",
			"GeForce GT 240",
			"GeForce GT 320 (OEM Product)",
			"GeForce GT 705 (OEM)",
			"GeForce GT 620 (OEM)",
			"GeForce GT 610",
			"GeForce GT 520 (OEM)",
			"GeForce GT 520",
			"GeForce GT 220",
			"GeForce 605 (OEM)",
			"GeForce 510 (OEM)",
			"GeForce 405 (OEM)",
			"GeForce 310 (OEM)"
		};
	}
}
