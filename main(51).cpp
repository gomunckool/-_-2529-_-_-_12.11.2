#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct Ticket {
    string punkt_priznachennya;
    string nomer_reysu;
    string PIB_pasazhyra;
    string data_vylotu;
};

struct TreeNode {
    Ticket data;
    TreeNode *left, *right;
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

Ticket vvedennyaZayavky() {
    Ticket t;
    cout << "Punkt priznachennya: "; cin >> t.punkt_priznachennya;
    cout << "Nomer reysu: "; cin >> t.nomer_reysu;
    cout << "PIB pasazhyra: ";
    clearInput();
    getline(cin, t.PIB_pasazhyra);
    cout << "Data (DD.MM.RRRR): "; cin >> t.data_vylotu;
    return t;
}

TreeNode* dodatyVTerevo(TreeNode* root, Ticket t) {
    if (!root) return new TreeNode{ t, nullptr, nullptr };
    if (t.nomer_reysu < root->data.nomer_reysu)
        root->left = dodatyVTerevo(root->left, t);
    else
        root->right = dodatyVTerevo(root->right, t);
    return root;
}

void pokazatyDerevo(TreeNode* root) {
    if (!root) return;
    pokazatyDerevo(root->left);
    cout << root->data.nomer_reysu << " | " << root->data.punkt_priznachennya 
         << " | " << root->data.PIB_pasazhyra << " | " << root->data.data_vylotu << endl;
    pokazatyDerevo(root->right);
}

void znaytyTaVydalyty(TreeNode*& root, string reys, string data) {
    if (!root) return;
    znaytyTaVydalyty(root->left, reys, data);
    znaytyTaVydalyty(root->right, reys, data);

    if (root->data.nomer_reysu == reys && root->data.data_vylotu == data) {
        cout << "Obrobleno: " << root->data.PIB_pasazhyra << endl;
        TreeNode* temp = root;
        if (!root->left) root = root->right;
        else if (!root->right) root = root->left;
        else {
            root = nullptr; 
        }
        delete temp;
    }
}

void derevoVVector(TreeNode* root, vector<Ticket>& v) {
    if (!root) return;
    derevoVVector(root->left, v);
    v.push_back(root->data);
    derevoVVector(root->right, v);
}

TreeNode* pobuduvatyZbalansovane(const vector<Ticket>& v, int start, int end) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    TreeNode* root = new TreeNode{ v[mid], nullptr, nullptr };
    root->left = pobuduvatyZbalansovane(v, start, mid - 1);
    root->right = pobuduvatyZbalansovane(v, mid + 1, end);
    return root;
}

void balansuvannya(TreeNode*& root) {
    vector<Ticket> v;
    derevoVVector(root, v);
    root = pobuduvatyZbalansovane(v, 0, v.size() - 1);
    cout << "Derevo zbalansovano.\n";
}

void zberezhtyUFail(TreeNode* root, ofstream& out) {
    if (!root) return;
    zberezhtyUFail(root->left, out);
    out << root->data.punkt_priznachennya << "\n" << root->data.nomer_reysu << "\n"
        << root->data.PIB_pasazhyra << "\n" << root->data.data_vylotu << "\n";
    zberezhtyUFail(root->right, out);
}

int main() {
    TreeNode* root = nullptr;
    int vibir;
    string f_name, reys, data;

    do {
        cout << "\n1. Dodaty 2. Vydalyty 3. Pokazaty 4. Balansuvaty 5. Zberezhty 6. Zavantazhyty 0. Vykhid\n>> ";
        if (!(cin >> vibir)) { cout << "Pomylka!"; clearInput(); continue; }

        switch (vibir) {
        case 1: root = dodatyVTerevo(root, vvedennyaZayavky()); break;
        case 2: 
            cout << "Reys: "; cin >> reys; 
            cout << "Data: "; cin >> data; 
            znaytyTaVydalyty(root, reys, data); 
            break;
        case 3: 
            if (!root) cout << "Derevo porozhnye.\n";
            else pokazatyDerevo(root); 
            break;
        case 4: 
            if (root) balansuvannya(root); 
            break;
        case 5: {
            cout << "Imya failu: "; cin >> f_name;
            ofstream out(f_name);
            zberezhtyUFail(root, out);
            out.close();
            cout << "Dani zberezheno.\n";
            break;
        }
        case 6: {
            cout << "Imya failu: "; cin >> f_name;
            ifstream in(f_name);
            if (!in) { cout << "Fail ne znaydeno.\n"; break; }
            Ticket t;
            while (getline(in, t.punkt_priznachennya) && getline(in, t.nomer_reysu) &&
                   getline(in, t.PIB_pasazhyra) && getline(in, t.data_vylotu)) {
                root = dodatyVTerevo(root, t);
            }
            in.close();
            cout << "Dani zavantazheno.\n";
            break;
        }
        }
    } while (vibir != 0);

    return 0;
}
