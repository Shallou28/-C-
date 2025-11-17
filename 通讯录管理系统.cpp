#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// 联系人结构体
struct Contact {
    string name;        // 姓名
    string phone;       // 电话
    string email;       // 邮箱
    string address;     // 地址
    string category;    // 分类（如：家人、朋友、同事等）
    
    // 构造函数
    Contact() {}
    Contact(string n, string p, string e, string a, string c) 
        : name(n), phone(p), email(e), address(a), category(c) {}
    
    // 显示联系人信息
    void display() const {
        cout << left << setw(15) << name 
             << setw(18) << phone 
             << setw(25) << email 
             << setw(30) << address 
             << setw(10) << category << endl;
    }
    
    // 检查是否匹配搜索条件
    bool matches(const string& keyword, const string& searchType) const {
        if (searchType == "name" || searchType == "all") {
            if (name.find(keyword) != string::npos) return true;
        }
        if (searchType == "phone" || searchType == "all") {
            if (phone.find(keyword) != string::npos) return true;
        }
        if (searchType == "email" || searchType == "all") {
            if (email.find(keyword) != string::npos) return true;
        }
        if (searchType == "address" || searchType == "all") {
            if (address.find(keyword) != string::npos) return true;
        }
        if (searchType == "category" || searchType == "all") {
            if (category.find(keyword) != string::npos) return true;
        }
        return false;
    }
};

// 通讯录管理类
class AddressBook {
private:
    vector<Contact> contacts;
    
public:
    // 添加联系人
    void addContact() {
        Contact contact;
        cout << "\n========== 添加联系人 ==========" << endl;
        cout << "姓名: ";
        cin.ignore();
        getline(cin, contact.name);
        cout << "电话: ";
        getline(cin, contact.phone);
        cout << "邮箱: ";
        getline(cin, contact.email);
        cout << "地址: ";
        getline(cin, contact.address);
        cout << "分类（家人/朋友/同事/其他）: ";
        getline(cin, contact.category);
        
        // 检查电话是否已存在
        for (const auto& c : contacts) {
            if (c.phone == contact.phone) {
                cout << "\n错误：该电话号码已存在！" << endl;
                return;
            }
        }
        
        contacts.push_back(contact);
        cout << "\n联系人添加成功！" << endl;
    }
    
    // 删除联系人
    void deleteContact() {
        if (contacts.empty()) {
            cout << "\n通讯录为空，无法删除！" << endl;
            return;
        }
        
        cout << "\n========== 删除联系人 ==========" << endl;
        string phone;
        cout << "请输入要删除的联系人电话: ";
        cin.ignore();
        getline(cin, phone);
        
        auto it = find_if(contacts.begin(), contacts.end(),
            [&phone](const Contact& c) { return c.phone == phone; });
        
        if (it != contacts.end()) {
            cout << "\n找到联系人：" << endl;
            displayHeader();
            it->display();
            cout << "\n确认删除？(y/n): ";
            char confirm;
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                contacts.erase(it);
                cout << "联系人删除成功！" << endl;
            } else {
                cout << "取消删除。" << endl;
            }
        } else {
            cout << "\n未找到该联系人！" << endl;
        }
    }
    
    // 修改联系人
    void modifyContact() {
        if (contacts.empty()) {
            cout << "\n通讯录为空，无法修改！" << endl;
            return;
        }
        
        cout << "\n========== 修改联系人 ==========" << endl;
        string phone;
        cout << "请输入要修改的联系人电话: ";
        cin.ignore();
        getline(cin, phone);
        
        auto it = find_if(contacts.begin(), contacts.end(),
            [&phone](const Contact& c) { return c.phone == phone; });
        
        if (it != contacts.end()) {
            cout << "\n当前联系人信息：" << endl;
            displayHeader();
            it->display();
            cout << "\n请输入新信息（直接回车保持原值）：" << endl;
            
            string input;
            cout << "姓名 [" << it->name << "]: ";
            getline(cin, input);
            if (!input.empty()) it->name = input;
            
            cout << "电话 [" << it->phone << "]: ";
            getline(cin, input);
            if (!input.empty()) {
                // 检查新电话是否与其他联系人冲突
                bool conflict = false;
                for (const auto& c : contacts) {
                    if (c.phone == input && &c != &(*it)) {
                        conflict = true;
                        break;
                    }
                }
                if (!conflict) {
                    it->phone = input;
                } else {
                    cout << "警告：该电话号码已存在，保持原值。" << endl;
                }
            }
            
            cout << "邮箱 [" << it->email << "]: ";
            getline(cin, input);
            if (!input.empty()) it->email = input;
            
            cout << "地址 [" << it->address << "]: ";
            getline(cin, input);
            if (!input.empty()) it->address = input;
            
            cout << "分类 [" << it->category << "]: ";
            getline(cin, input);
            if (!input.empty()) it->category = input;
            
            cout << "\n联系人修改成功！" << endl;
        } else {
            cout << "\n未找到该联系人！" << endl;
        }
    }
    
    // 查询联系人
    void searchContact() {
        if (contacts.empty()) {
            cout << "\n通讯录为空，无法查询！" << endl;
            return;
        }
        
        cout << "\n========== 查询联系人 ==========" << endl;
        cout << "请选择查询方式：" << endl;
        cout << "1. 按姓名查询" << endl;
        cout << "2. 按电话查询" << endl;
        cout << "3. 按邮箱查询" << endl;
        cout << "4. 按地址查询" << endl;
        cout << "5. 按分类查询" << endl;
        cout << "6. 综合查询（所有字段）" << endl;
        cout << "请选择 (1-6): ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        string searchType;
        switch (choice) {
            case 1: searchType = "name"; break;
            case 2: searchType = "phone"; break;
            case 3: searchType = "email"; break;
            case 4: searchType = "address"; break;
            case 5: searchType = "category"; break;
            case 6: searchType = "all"; break;
            default:
                cout << "无效的选择！" << endl;
                return;
        }
        
        string keyword;
        cout << "请输入搜索关键词: ";
        getline(cin, keyword);
        
        vector<Contact> results;
        for (const auto& contact : contacts) {
            if (contact.matches(keyword, searchType)) {
                results.push_back(contact);
            }
        }
        
        if (results.empty()) {
            cout << "\n未找到匹配的联系人！" << endl;
        } else {
            cout << "\n找到 " << results.size() << " 个匹配的联系人：" << endl;
            displayHeader();
            for (const auto& contact : results) {
                contact.display();
            }
        }
    }
    
    // 显示所有联系人
    void displayAll() {
        if (contacts.empty()) {
            cout << "\n通讯录为空！" << endl;
            return;
        }
        
        cout << "\n========== 所有联系人 ==========" << endl;
        cout << "共 " << contacts.size() << " 个联系人" << endl;
        displayHeader();
        for (const auto& contact : contacts) {
            contact.display();
        }
    }
    
    // 按分类显示
    void displayByCategory() {
        if (contacts.empty()) {
            cout << "\n通讯录为空！" << endl;
            return;
        }
        
        cout << "\n========== 按分类显示 ==========" << endl;
        string category;
        cout << "请输入分类名称: ";
        cin.ignore();
        getline(cin, category);
        
        vector<Contact> results;
        for (const auto& contact : contacts) {
            if (contact.category == category) {
                results.push_back(contact);
            }
        }
        
        if (results.empty()) {
            cout << "\n该分类下没有联系人！" << endl;
        } else {
            cout << "\n分类 \"" << category << "\" 下有 " << results.size() << " 个联系人：" << endl;
            displayHeader();
            for (const auto& contact : results) {
                contact.display();
            }
        }
    }
    
    // 显示所有分类
    void displayCategories() {
        if (contacts.empty()) {
            cout << "\n通讯录为空！" << endl;
            return;
        }
        
        vector<string> categories;
        for (const auto& contact : contacts) {
            bool found = false;
            for (const auto& cat : categories) {
                if (cat == contact.category) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                categories.push_back(contact.category);
            }
        }
        
        cout << "\n========== 所有分类 ==========" << endl;
        cout << "共有 " << categories.size() << " 个分类：" << endl;
        for (size_t i = 0; i < categories.size(); i++) {
            int count = 0;
            for (const auto& contact : contacts) {
                if (contact.category == categories[i]) {
                    count++;
                }
            }
            cout << (i + 1) << ". " << categories[i] << " (" << count << " 个联系人)" << endl;
        }
    }
    
    // 显示表头
    void displayHeader() const {
        cout << "\n" << left 
             << setw(15) << "姓名" 
             << setw(18) << "电话" 
             << setw(25) << "邮箱" 
             << setw(30) << "地址" 
             << setw(10) << "分类" << endl;
        cout << string(98, '-') << endl;
    }
    
    // 保存到文件
    void saveToFile(const string& filename = "address_book.txt") {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "\n无法打开文件进行保存！" << endl;
            return;
        }
        
        for (const auto& contact : contacts) {
            file << contact.name << "|"
                 << contact.phone << "|"
                 << contact.email << "|"
                 << contact.address << "|"
                 << contact.category << "\n";
        }
        
        file.close();
        cout << "\n数据已保存到 " << filename << endl;
    }
    
    // 从文件加载
    void loadFromFile(const string& filename = "address_book.txt") {
        ifstream file(filename);
        if (!file.is_open()) {
            return; // 文件不存在，不报错
        }
        
        contacts.clear();
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            Contact contact;
            
            getline(ss, contact.name, '|');
            getline(ss, contact.phone, '|');
            getline(ss, contact.email, '|');
            getline(ss, contact.address, '|');
            getline(ss, contact.category, '|');
            
            contacts.push_back(contact);
        }
        
        file.close();
    }
    
    // 获取联系人数量
    int getCount() const {
        return contacts.size();
    }
};

// 显示主菜单
void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "        通讯录管理系统" << endl;
    cout << "========================================" << endl;
    cout << "1. 添加联系人" << endl;
    cout << "2. 删除联系人" << endl;
    cout << "3. 修改联系人" << endl;
    cout << "4. 查询联系人" << endl;
    cout << "5. 显示所有联系人" << endl;
    cout << "6. 按分类显示" << endl;
    cout << "7. 显示所有分类" << endl;
    cout << "8. 保存数据到文件" << endl;
    cout << "9. 从文件加载数据" << endl;
    cout << "0. 退出程序" << endl;
    cout << "========================================" << endl;
    cout << "请选择操作 (0-9): ";
}

int main() {
    AddressBook addressBook;
    
    // 程序启动时自动加载数据
    addressBook.loadFromFile();
    if (addressBook.getCount() > 0) {
        cout << "已加载 " << addressBook.getCount() << " 个联系人" << endl;
    }
    
    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                addressBook.addContact();
                break;
            case 2:
                addressBook.deleteContact();
                break;
            case 3:
                addressBook.modifyContact();
                break;
            case 4:
                addressBook.searchContact();
                break;
            case 5:
                addressBook.displayAll();
                break;
            case 6:
                addressBook.displayByCategory();
                break;
            case 7:
                addressBook.displayCategories();
                break;
            case 8:
                addressBook.saveToFile();
                break;
            case 9:
                addressBook.loadFromFile();
                cout << "已加载 " << addressBook.getCount() << " 个联系人" << endl;
                break;
            case 0:
                cout << "\n感谢使用通讯录管理系统！" << endl;
                // 退出前自动保存
                addressBook.saveToFile();
                return 0;
            default:
                cout << "\n无效的选择，请重新输入！" << endl;
                break;
        }
    }
    
    return 0;
}

