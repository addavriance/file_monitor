# File Monitor
This program monitors a specified directory for files matching certain criteria (e.g., names containing specific substrings and having specific extensions). It logs the paths of these files and optionally deletes them.

## Features
- Monitors a directory (default: `C:/`) for blacklisted files.
- Deletes files matching the criteria automatically.
- Simple setup and lightweight.

## Requirements
- Windows operating system with the built-in compiler (Microsoft C++ Build Tools).
- Basic familiarity with command-line usage.

## Usage
### Step 1: Compiling the Program
1. **Ensure you have the required tools installed:**
    - You need the `cl.exe` compiler that comes with Microsoft C++ Build Tools (part of Visual Studio or the Windows SDK).

2. **Prepare the source files:**
    - Place the provided files (`main.cpp`, `FileMonitor.h`, `FileMonitor.cpp`, `FileCriteria.h`, `FileCriteria.cpp`) in the same folder.

3. **Open the command prompt in the source directory:**
    - Use **Shift + Right-Click** on the folder containing the files and select **“Open Command Window Here.”**

4. **Compile the program:**  
   Run the following command to compile the program and output it to a folder named `compiled` in the current directory:
   ```cmd
   mkdir compiled
   cl /EHsc /Fe:compiled\FileMonitor.exe main.cpp FileMonitor.cpp FileCriteria.cpp
   ```  

5. **Check the `compiled` folder:**  
   If the compilation is successful, the executable (`FileMonitor.exe`) will appear in the `compiled` folder.

---

### Step 2: Running the Program
1. Open a command prompt in the `compiled` folder where the `FileMonitor.exe` resides.
2. Run the executable:
   ```cmd
   FileMonitor.exe
   ```  
3. The program will monitor the `C:/` drive by default for blacklisted files matching the criteria set in the code (e.g., names containing "fba" and ending with `.json`).

---

## Important Notes
1. **Precompiled Executable:**  
   While I’ve provided a precompiled executable, it may not work correctly on all systems due to differences in environment and configurations. For the best results, compile the program yourself using the instructions above.

2. **Customization:**
    - To monitor a different directory, modify this line in the code:
      ```cpp
      FileMonitor fileMonitor(L"C:\\", criteriaList);
      ```  
      Replace `"C:\\"` with the path you wish to monitor.
    - To add more file criteria, modify the `criteriaList` in the code:
      ```cpp
      std::vector<FileCriteria> criteriaList = {
          FileCriteria(L"fba", L".json"), // add more patterns here
      };
      ```  

3. **Use at Your Own Risk:**  
   This program automatically deletes files matching the specified criteria. Ensure you’ve reviewed and understood the criteria before running it.

---

## Support
For any issues or suggestions, feel free to create an issue on the GitHub repository.