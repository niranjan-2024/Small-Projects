#include <bits/stdc++.h>
using namespace std;

class DelhiMetroMap {
public:
    DelhiMetroMap() {
        StationsVer = std::unordered_map<std::string, Vertex>();
    }

    class Vertex {
    public:
        std::unordered_map<std::string, int> neighbours;
    };

    // FUNCTIONS FOR VERTICES StationsVer
    std::unordered_map<std::string, Vertex> StationsVer;

    int numOfVertex() {
        return StationsVer.size();
    }

    bool containsStations(const std::string& vname) {
        return StationsVer.find(vname) != StationsVer.end();
    }

    void addVertex(const std::string& vname) {
        Vertex vtx;
        StationsVer[vname] = vtx;
    }

    void removeVertex(const std::string& vname) {
        Vertex vtx = StationsVer[vname];
        std::vector<std::string> keys;
        for (const auto& pair : vtx.neighbours) {
            keys.push_back(pair.first);
        }
        for (const std::string& key : keys) {
            Vertex& nbrVtx = StationsVer[key];
            nbrVtx.neighbours.erase(vname);
        }
        StationsVer.erase(vname);
    }

    // FUNCTIONS FOR EDGES
    int numOfEdges() {
        int count = 0;
        for (const auto& pair : StationsVer) {
            count += pair.second.neighbours.size();
        }
        return count / 2;
    }

    bool containsEdge(const std::string& vname1, const std::string& vname2) {
        if (StationsVer.find(vname1) == StationsVer.end() || StationsVer.find(vname2) == StationsVer.end())
            return false;

        const Vertex& vtx1 = StationsVer[vname1];
        return vtx1.neighbours.find(vname2) != vtx1.neighbours.end();
    }

    void addEdge(const std::string& vname1, const std::string& vname2, int value) {
        if (!containsEdge(vname1, vname2)) {
            Vertex& vtx1 = StationsVer[vname1];
            Vertex& vtx2 = StationsVer[vname2];
            vtx1.neighbours[vname2] = value;
            vtx2.neighbours[vname1] = value;
        }
    }

    void removeEdge(const std::string& vname1, const std::string& vname2) {
        if (containsEdge(vname1, vname2)) {
            Vertex& vtx1 = StationsVer[vname1];
            Vertex& vtx2 = StationsVer[vname2];
            vtx1.neighbours.erase(vname2);
            vtx2.neighbours.erase(vname1);
        }
    }

    // DISPLAYING FUNCTIONS
    void displayMap() {
        std::cout << "\t Delhi Metro Map\n";
        std::cout << "\t------------------\n";
        for (const auto& station : StationsVer) {
            std::cout << station.first << " =>\n";
            const Vertex& vtx = station.second;
            for (const auto& neighbor : vtx.neighbours) {
                std::cout << "\t" << neighbor.first;
                if (neighbor.first.length() < 16) std::cout << "\t";
                if (neighbor.first.length() < 8) std::cout << "\t";
                std::cout << neighbor.second << "\n";
            }
        }
        std::cout << "\t------------------\n";
    }

    void displayStations() {
        std::cout << "\n***********************************************************************\n";
        int i = 1;
        for (const auto& station : StationsVer) {
            std::cout << i << ". " << station.first << "\n";
            i++;
        }
        std::cout << "\n***********************************************************************\n";
    }

    // Normal DFS
    bool hasPath(const std::string& vname1, const std::string& vname2, std::unordered_map<std::string, bool>& vis) {
        if (containsEdge(vname1, vname2))
            return true;

        vis[vname1] = true; // mark as visited

        const Vertex& vtx = StationsVer[vname1];
        for (const auto& neighbor : vtx.neighbours) {
            if (!vis[neighbor.first] && hasPath(neighbor.first, vname2, vis)) {
                return true;
            }
        }
        return false;
    }

    // DjPair class for Priority Queues
    class DjPair {
    public:
        std::string vname;
        int distCost;

        DjPair() : vname(""), distCost(0) {} // Default constructor

        DjPair(const std::string& name, int cost) : vname(name), distCost(cost) {}
    };

    struct DjPairComparator {
        bool operator()(const DjPair& a, const DjPair& b) const {
            return a.distCost > b.distCost;
        }
    };

    // Shortest Path algo = Dijkstra's
    int dijkstra(const std::string& src, const std::string& des, bool nan) {
        int val = 0;
        std::unordered_map<std::string, DjPair> unVisited;
        std::priority_queue<DjPair, std::vector<DjPair>, DjPairComparator> queue;

        for (const auto& station : StationsVer) {
            DjPair newPair(station.first, std::numeric_limits<int>::max());
            if (station.first == src) {
                newPair.distCost = 0;
            }
            queue.push(newPair);
            unVisited[station.first] = newPair;
        }

        // Keep removing the pairs while the queue is not empty
        while (!queue.empty()) {
            DjPair curr = queue.top();
            queue.pop();
            if (curr.vname == des) {
                val = curr.distCost;
                break;
            }
            unVisited.erase(curr.vname);

            const Vertex& v = StationsVer[curr.vname];
            for (const auto& neighbor : v.neighbours) {
                if (unVisited.find(neighbor.first) != unVisited.end()) {
                    int oldDist = unVisited[neighbor.first].distCost;
                    int newDist = curr.distCost + neighbor.second;
                    if (newDist < oldDist) {
                        DjPair updatedPair(neighbor.first, newDist);
                        unVisited[neighbor.first] = updatedPair;
                        queue.push(updatedPair);
                    }
                }
            }
        }

        if (nan) return -1; // Not a neighbor

        if (val == 0) val = -2; // No path found

        return val;
    }

    std::string getMinimumDistance(const std::string& vname1, const std::string& vname2) {
        std::unordered_map<std::string, bool> vis;
        if (!containsStations(vname1) || !containsStations(vname2)) {
            return "Station not found in the map.";
        }
        bool hasP = hasPath(vname1, vname2, vis);
        if (!hasP) {
            return "No path found between the stations.";
        }
        else {
            int ans = dijkstra(vname1, vname2, false);
            return "The minimum distance between " + vname1 + " and " + vname2 + " is " + std::to_string(ans) + " km.";
        }
    }

    std::vector<std::string> getInterchanges(const std::string& vname) {
        if (!containsStations(vname)) {
            std::vector<std::string> emptyList;
            emptyList.push_back("Station not found in the map.");
            return emptyList;
        }
        std::unordered_map<std::string, bool> vis;
        std::vector<std::string> result;
        vis[vname] = true;
        const Vertex& vtx = StationsVer[vname];
        for (const auto& neighbor : vtx.neighbours) {
            if (!vis[neighbor.first]) {
                if (neighbor.second == 0) { // 0 km distance implies interchange
                    result.push_back(neighbor.first);
                }
            }
        }
        if (result.empty()) {
            result.push_back("No interchanges found for the station.");
        }
        return result;
    }
};

int main()
{
   string name;
   cout<<"Enter Name"<<endl;
   cin>>name;
   
   int password;
   cout<<"Enter password"<<endl;
   cin>>password;
   
   if(name == "tarun" && password == 0000){
      cout<<"you have Successfully Logged In"<<endl;
   }
   else{
      cout<<"your password or name is incorrect, so run program again"<<endl;
      return 0;
   }
   
   vector<string> stations = {"Noida Sector 62~B","Yamuna Bank~B","Vaishali~B","Janak Puri West~BO","Huda City Center~Y","Vishwavidyalaya~Y","New Delhi~YO","Shivaji Stadium~O","IGI Airport~O","Netaji Subhash Place~PR"};
   
    DelhiMetroMap delhiMetro;

    // Add stations
    delhiMetro.addVertex("Delhi");
    delhiMetro.addVertex("New Delhi");
    delhiMetro.addVertex("Old Delhi");
    delhiMetro.addVertex("Kashmere Gate");
    delhiMetro.addVertex("Rajiv Chowk");
    delhiMetro.addVertex("Lajpat Nagar");
    delhiMetro.addVertex("Central Secretariat");
    delhiMetro.addVertex("Khan Market");
    delhiMetro.addVertex("Jor Bagh");

    // Add connections (edges)
    delhiMetro.addEdge("Delhi", "New Delhi", 4);
    delhiMetro.addEdge("Delhi", "Old Delhi", 6);
    delhiMetro.addEdge("New Delhi", "Rajiv Chowk", 2);
    delhiMetro.addEdge("Rajiv Chowk", "Central Secretariat", 0); // Interchange
    delhiMetro.addEdge("Rajiv Chowk", "Khan Market", 3);
    delhiMetro.addEdge("Khan Market", "Jor Bagh", 1);
   
 while(true){
    int n;
   
    cout<<"Welcome to Delhi Metro"<<endl;
    cout<<"\t\t\t\t~~How Can I Help You ~~"<<endl;
	cout<<"1. LIST ALL THE BIG STATIONS"<<endl;
	cout<<"2. SHOW THE METRO MAP"<<endl;
	cout<<"3. GET Shortest DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION"<<endl;
	cout<<"4. GET Shortest TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION"<<endl;
	cout<<"5. GET Shortest PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION"<<endl;
	cout<<"6. EXIT THE MENU"<<endl;
	cout<<"ENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 6) : "<<endl;
	
    cin>>n;
     
   if(n<1 || n>6){
       cout<<"Invalid choice run program again"<<endl;
    //   return 0;
   }
   else{
        cout<<"you chosen "<<n<<endl;
   }
   
   if(n == 1){
       cout<<"Big Stations are:"<<endl;
       for(int i=0;i<stations.size();i++){
           cout<<stations[i]<<endl;
       }
       cout<<"Your Choice is FullFilled"<<endl;
       cout<<endl;
       cout<<endl;
   }
   else if(n == 2){
       delhiMetro.displayMap();
       cout<<"Your Choice is FullFilled"<<endl;
       cout<<endl;
       cout<<endl;
   }
   else if(n == 3){
        cout<<"Here we have chose starting station as Delhi and ending at Central Secretrariat if you want to explore more then change stations implicitly in the program that will be better for this command type program"<<endl;
        std::string startStation = "Delhi";
        std::string endStation = "Central Secretariat";

        // Get the minimum distance between two stations
        std::string result = delhiMetro.getMinimumDistance(startStation, endStation);
        std::cout << result << std::endl;

        // Get interchanges at a station
        std::string interchangeStation = "Rajiv Chowk";
        std::vector<std::string> interchanges = delhiMetro.getInterchanges(interchangeStation);
        std::cout << "Interchanges at " << interchangeStation << ":\n";
        for (const std::string& interchange : interchanges) {
            std::cout << interchange << std::endl;
        }
        
       cout<<"Your Choice is FullFilled"<<endl;
       cout<<endl;
       cout<<endl;
   }
   else if(n == 4){
       cout<<"Getting Shorter time is under maintaince"<<endl;
       cout<<"Your Choice is FullFilled"<<endl;
       cout<<endl;
       cout<<endl;
   }
   else if(n == 5){
       cout<<"Getting Shorter distance in kilometers is under maintainance"<<endl;
       cout<<"Your Choice is FullFilled"<<endl;
       cout<<endl;
       cout<<endl;
   }
   else{
       cout<<"Thanks for using the Application"<<endl;
       cout<<"Your Choice is FullFilled"<<endl;
       return 0;
   }
 }
   
   return 0;
}
