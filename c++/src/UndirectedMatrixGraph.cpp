////
//// Created by Mian Hashim Shah on 4/4/18.
////

#include "UndirectedMatrixGraph.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include <limits> // double max value
#include <stack>

#include "CurrencyPair.h"

// INF represents no-edge
static const double INF = std::numeric_limits<double>::max();

//constructor of undirected graph using adjacency matrix
template<class T>
UndirectedMatrixGraph<T>::UndirectedMatrixGraph() : Graph<T>(), adjMatrix(), verticesMap() {}


//This function adds a vertices to our vertex List
// @param: Vertex * addThisVertex

//1. adds to list using stl vector function
//2. notifies user
//3. increments the number of vertices
template<class T>
void UndirectedMatrixGraph<T>::addVertex(const T& value)
{

    if (lookUpVertex(value) != -1)
        return;

    Vertex<T> vertex(value);
    vertexList.push_back(vertex);

    unsigned long insertedIndex = vertexList.size() - 1;

    // save the inserted value and it's index in the map
    verticesMap.insert(
            std::make_pair(
                    (std::is_same<T, std::string>::value) ? (value) : std::string(value), insertedIndex
            )
    );

//    std::cout << __FUNCTION__ << ": Added vertex "   << "\n";

    unsigned int prevCount = totalNumberOfVertices;
    totalNumberOfVertices++;

    // resize the vectors (as we add a new vertex, columns increase)
    for (auto it = adjMatrix.begin(); it != adjMatrix.end(); ++it) {
        while (it->size() < totalNumberOfVertices)
            it->push_back(INF); // set the distance to INF
    }

    // create a new vertex vector
    auto v = std::vector<double>();
    for (int i = 0; i < totalNumberOfVertices; ++i) // initialize the vector to INF with size: numberOfVertices
        v.push_back( i == insertedIndex ? 0 : INF); // vertex distance to itself should be 0

    adjMatrix.push_back(v);

};

//This function removes a vertices to our vertex List
// @param: Vertex * deleteThisVertex

//1. Checks for whether the vertex existss, if it does, erase from list using stl vector function
//2. notifies user
//3. increments the number of vertices
template<class T>
void UndirectedMatrixGraph<T>::removeVertex(const T& value)
{
    int index = lookUpVertex(value);

    if (index != -1 && totalNumberOfVertices!=-1) // if index is valid
    {
        vertexList.erase(vertexList.begin() + index); // remove vertex at that index
        totalNumberOfVertices--;

        //resize the matrix accordingly
        adjMatrix.erase(adjMatrix.begin() + index);
        for (auto iterator = adjMatrix.begin(); iterator != adjMatrix.end(); ++iterator)
        {
            iterator->erase(iterator->begin() + index);
        }

        // remove the value from the map as well
        verticesMap.erase(std::string(value));

        std::cout << __FUNCTION__ << ": Removed vertex at index " << index << "\n";
    }
}

//This function adds an edge between two given vertices and sets an associated cost to the edge
// @param: Vertex * fromVertex, Vertex * toVertex

//1. safe check if the passed vertices are valid
//2. look up in our vertex list if these vertices exist. Return index of both vertices
//3. check if edge already exists between them, if yes, then return
//4. add edge between the vertices
template<class T>
void UndirectedMatrixGraph<T>::addEdge(const T& fromValue, const T& toValue, double cost)
{

    //look up vertices if they exist
    int fromIndex = lookUpVertex(fromValue);
    int toIndex = lookUpVertex(toValue);

    if (fromIndex != -1 && toIndex != -1)
    {
        // if they exist, then add it
        adjMatrix[fromIndex][toIndex] = adjMatrix[toIndex][fromIndex] = cost; // add edge between vertices
    }
}
//This function removes an edge between two given vertices
// @param: Vertex * fromVertex, Vertex * toVertex

//1. safe check if the passed vertices are valid
//2. look up in our vertex list if these vertices exist. Return index of both vertices
//3. check if edge already exists between them, if not, then nothing to delete. We exit then
//4. remove edge between the vertices
template<class T>
void UndirectedMatrixGraph<T>::removeEdge(const T& fromValue, const T& toValue)
{
    //look up vertices if they exist
    int fromIndex = lookUpVertex(fromValue);
    int toIndex = lookUpVertex(toValue);

    if (fromIndex != -1 && toIndex != -1) {
        //check to see if edge doesnt exist between vertices
        if (adjMatrix[fromIndex][toIndex] == 0) {
            std::cout << __FUNCTION__ << ": Edge does not exist.. Nothing to do here" << "\n";
            return;
        }

        // if they exist, then remove it
        adjMatrix[fromIndex][toIndex] = adjMatrix[toIndex][fromIndex] = 0.00; //add edge between vertices
    }
}


//This function checks to see if a vertex exists or not
// @param: Vertex * V

//1. safe check if the passed vertices are valid
//2. check if unordered_map contains specified value.
// if yes, return the associated value (index).
//3. otherwise return -1 to indicate "not found"
template<class T>
int UndirectedMatrixGraph<T>::lookUpVertex(const T& value) const
{
    //
    auto iterator = verticesMap.find(value);

    if (iterator == verticesMap.end()) { // if iterator points to the end of map, element is not in the map
        return -1;
    }

    return iterator->second;
}

//This function returns the weight between two vertices.
//@param: const T &fromValue, const T &toValue
//returns double in the form of the weight
template <class T>
double UndirectedMatrixGraph<T>::getWeight(const T &fromValue, const T &toValue) const
{
    int fromIndex = lookUpVertex(fromValue);
    int toIndex = lookUpVertex(toValue);

    if (fromIndex != -1 && toIndex != -1) //if both vertices exist
    {
        return adjMatrix[fromIndex][toIndex]; //return weight
    }
    return -1; //if vertices arent available, return -1
}

//This function returns the neighbors of a specified vertex
//@param: const T& targetCoin
//returns list of neighbors to the targetCoin
template<class T>
std::vector< Vertex<T> > UndirectedMatrixGraph<T>::getNeighbors(const T &targetCoin)
{
    std::vector< Vertex<T> > listOfNeighbors;//create a list

    int index = lookUpVertex(targetCoin); //gets index of vertex
    if (index != -1) //if vertex exists
    {
        for (int i = 0; i < this->getNumberOfVertices(); i++)//loop through the rows of matrix
        {
            if (adjMatrix[index][i] != INF) //if a valid edge exists to the passed vertex
            {
                listOfNeighbors.push_back(vertexList[i]);//push the neighbors to the list
            }
        }
    }
    return listOfNeighbors; // return list of neighbors
}


/*! toString - create a string representation of graph with all vertices
 *
 * @tparam T - type of the object that this graph holds
 * @return - string representation of this graph (in tabular format)
 */
template<class T>
std::string UndirectedMatrixGraph<T>::toString()
{
    // assumed length of crypto currency
    static const int baseSymbolLength = 3;

    // amount of spaces between values/vertices
    static const int spacing = 4;

    // create space strings that are used when printing
    std::string spaces = std::string(spacing, ' ');
    std::string headerSpaces = std::string(spacing + 1, ' ');
    std::string str(spacing + 3, ' ');

    // row with headers
    for (auto it = vertexList.begin(); it != vertexList.end(); ++it) {
        str += (*it).getValue() + headerSpaces;
    }

    // remove the last whitespaces
    if (str.length() > 0)
        str = str.substr(0, str.length() - spacing);

    str += "\n";

    std::stringstream buffer;
    int i = 0; // counter of vertex list
    std::string line; // hold data for each line

    for (auto it = adjMatrix.begin(); it != adjMatrix.end(); ++it) {
        line += vertexList.at(i++).getValue(); // get the symbol
        line += std::string(spacing - (line.length() - baseSymbolLength), ' '); // calculate the spacing based on the symbol length
        for (auto column = it->begin(); column != it->end(); ++column) {
            buffer.str(std::string());
            const double value = (*column == INF) ? -1 : *column;
            if (value != -1)
                buffer << std::fixed << std::setprecision(2) << value << spaces; // get value of double with precision of 2
            else
                buffer << "INFF" << spaces; // get value of double with precision of 2

            line += buffer.str();
        }
        str += line + "\n\n";
        line = "";
    }

    return str;
}


/*! reset - clear the values in the graph
 *
 * @tparam T - the type of the objects that Graph holds
 */
template<class T>
void UndirectedMatrixGraph<T>::reset() {
    vertexList.clear();
    adjMatrix.clear();
    totalNumberOfVertices = 0;
}



/*! computeShortestDistanceBetweenAllVertices - Calculate shortest paths between all vertices using Floyd-Warshall Algorithm
 *
 * @tparam T - the object type that Graph holds
 * @return 2D vector with shortest paths between all vertices
 */
template<class T>
std::vector<std::vector<double> > UndirectedMatrixGraph<T>::computeShortestDistanceBetweenAllVertices() const {

    // get the number of vertices
    const unsigned int V = this->getNumberOfVertices();
    // copy the matrix of current distances. this 2D vector will contain the shortest paths after running Floyd-Warshall Algorithm
    std::vector< std::vector<double> > dists(adjMatrix);

    // implementation of the all-pairs-short algorithm
    for (int intermediateVertex = 0; intermediateVertex < V; ++intermediateVertex) {

        // Pick all vertices as source one by one
        for (int sourceVertex = 0; sourceVertex < V; ++sourceVertex) {

            // Pick all vertices as destination for the
            // above picked source
            for (int destinationVertex = 0; destinationVertex < V; ++destinationVertex) {

                if (
                        dists[sourceVertex][intermediateVertex] != INF && // avoid overflow before summing up
                        dists[intermediateVertex][destinationVertex] != INF &&
                        dists[sourceVertex][intermediateVertex] + dists[intermediateVertex][destinationVertex]
                            < dists[sourceVertex][destinationVertex] // check if the sum is smaller than actual path
                        )
                {
                    // save the distance in the matrix
                    dists[sourceVertex][destinationVertex] = dists[sourceVertex][intermediateVertex] + dists[intermediateVertex][destinationVertex];
                }
            }
        }
    }
    // print the resulting vector
    /*
    for (int l = 0; l < dists.size();  ++l) {
        for (int i = 0; i < dists.size();  ++i) {
            const auto value = dists[l][i] == INF ? 0 : dists[l][i];
            std::cout << value << "  ";
        }
        std::cout << "\n\n";
    }
    */

    return dists;
}




/*! computeShortestDistanceBetweenVertices - Calculate shortest paths between two searched vertices using Floyd-Warshall Algorithm
 *
 * @param from - source vertex (from which calculate distance)
 * @param to - destination vertex (to which calculate distance)
 * @return list with shortest paths between given vertices.
 *         If those vertices do not exist in the graph, return empty list
 */
template<class T>
std::list<CurrencyPair> UndirectedMatrixGraph<T>::computeShortestDistanceBetweenVertices(const T& from, const T& to) const {

    // list with pairs of currencies that we return
    std::list<CurrencyPair> pairs;

    // Pairs found by the all-pairs shortest algorithm
    // TODO: Rename consideredPairs to a more appropriate identifier
    std::stack<CurrencyPair> consideredPairs;

    // first check if vertices with given values exist in the graph
    const int sourceIndex = lookUpVertex(from);
    const int destIndex = lookUpVertex(to);

    // if the from or to target vertices is not present in the graph,
    // return empty vector because we do not need to iterate through the graph
    if (sourceIndex == -1 || destIndex == -1)
        return pairs;


    // get the number of vertices
    const unsigned int V = this->getNumberOfVertices();
    // copy the matrix of current distances. this 2D vector will contain the shortest paths after running Floyd-Warshall Algorithm
    std::vector< std::vector<double> > dists(adjMatrix);


    // implementation of the all-pairs-short algorithm
    for (int intermediateVertex = 0; intermediateVertex < V; ++intermediateVertex) {

        // Pick all vertices as source one by one
        for (int sourceVertex = 0; sourceVertex < V; ++sourceVertex) {

            // Pick all vertices as destination for the
            // above picked source
            for (int destinationVertex = 0; destinationVertex < V; ++destinationVertex) {

                if (
                        dists[sourceVertex][intermediateVertex] != INF && // avoid overflow before summing up
                        dists[intermediateVertex][destinationVertex] != INF &&
                        dists[sourceVertex][intermediateVertex] + dists[intermediateVertex][destinationVertex]
                        < dists[sourceVertex][destinationVertex] // check if the sum is smaller than actual path
                        )
                {
                    // save the distance in the matrix
                    dists[sourceVertex][destinationVertex] = dists[sourceVertex][intermediateVertex] + dists[intermediateVertex][destinationVertex];

//                    if (vertexList[sourceVertex].getValue() == from && vertexList[destinationVertex].getValue() == to) {
//                        std::cout << "[" << vertexList[sourceVertex].getValue() << "]"
//                                  << " - [" << vertexList[intermediateVertex].getValue() << "]"
//                                  << " - [" << vertexList[destinationVertex].getValue() << "]\n";
//                    }

                    // and put the pair into the queue
                    if (vertexList[sourceVertex].getValue() == from) {
                        auto fromS = vertexList[sourceVertex].getValue();
                        auto intermidS = vertexList[intermediateVertex].getValue();
                        auto destS = vertexList[destinationVertex].getValue();

                        consideredPairs.emplace(fromS, intermidS, dists[sourceVertex][intermediateVertex]);
                        consideredPairs.emplace(intermidS, destS, dists[intermediateVertex][destinationVertex]);
                    }
                }
            }
        }


    }

    // Parse through the stack of pairs

    // prevSource and prevDestination keeps track of the from & to symbols from the previous loop iteration
    // They're used to keep track of the pattern of pairs
    auto prevSource = from;
    auto prevDestination = to;

    // Ensures that the pair being checked is relevant to the shortest path
    bool isRelevant = true;


    // Holds a temporary stack that stores what will become the list of pairs
    std::stack<CurrencyPair> temp;
    while (!consideredPairs.empty()) {
        auto pair = consideredPairs.top();
        consideredPairs.pop();
        

        // Check if the pair belongs in the shortest path
        if ((prevSource == pair.getFromSymbol() ||
            prevSource == pair.getToSymbol() || 
            prevDestination == pair.getFromSymbol() ||
            prevDestination == pair.getToSymbol()) &&
            isRelevant
            ) 
        {
            temp.push(pair);
        } 
        else 
            isRelevant = false;

        if(pair.getFromSymbol() == from && pair.getToSymbol() == to) {
            // Empty the temp stack
            while (!temp.empty())
                temp.pop();
            
            isRelevant = true;
        }

        prevSource = pair.getFromSymbol();
        prevDestination = pair.getToSymbol();
    }

    // Insert temp stack into pairs list
    int topIndex = temp.size();
    while(temp.size() > 0) {
        CurrencyPair pair = temp.top();

        if ((pair.getFromSymbol() == from && temp.size() != topIndex) || pair.getToSymbol() == to && temp.size() > 1) {

        } else {
            pairs.push_back(pair);
        }

        temp.pop();
    }

    // if we found pairs, we should return it now
    if (!pairs.empty())
        return pairs;

    // if not, we still need to check if the graph has the direct cost for our target pair
    if (dists[sourceIndex][destIndex] != INF) {
        // if the distance is not INF, then return this pair
        pairs.emplace_back(vertexList[sourceIndex].getValue(), vertexList[destIndex].getValue(), dists[sourceIndex][destIndex]);
    }

    return pairs;
}




template<class T>
void UndirectedMatrixGraph<T>::constructPath(int parent[], int j, std::list<T>& path) const {
    // Base Case : If j is source
    if (parent[j] == - 1)
        return;

    this->constructPath(parent, parent[j], path);

    path.push_back(vertexList[j].getValue());
}



template<class T>
int UndirectedMatrixGraph<T>::minDistance(const double *dist, const bool *sptSet, int V) const {
    // initialize min value
    double min = INF;
    int min_index = 0;

    for (int v = 0; v < V; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}






template<class T>
std::list<CurrencyPair> UndirectedMatrixGraph<T>::getShortestPairsBetween(const T& from, const T& to) const {
    // list with pairs of currencies that we return
    std::list<CurrencyPair> pairs;

    // find the index of searched values in the graph
    const int src = lookUpVertex(from);
    const int dest = lookUpVertex(to);

    // if the source or destination vertex is not in the graph, we just return empty list
    if (src == -1 || dest == -1)
        return pairs;


    // V - number of vertices
    int V = this->getNumberOfVertices();

    // array distances[] will hold the shortest distance from source to all vertices
    double distances[V];


    // shortestPathTreeVisited[i] will be true if vertex i is included in shortest distance
    // from src to i
    bool shortestPathTreeVisited[V];

    // store shortest path tree
    int parentVertexArray[V];

    // initialize
    for (int i = 0; i < V; i++)
    {
        parentVertexArray[src] = -1;
        distances[i] = INF;
        shortestPathTreeVisited[i] = false;
    }

    // distance of source vertex from itself is 0
    distances[src] = 0;

    // find shortest path for all vertices
    for (int count = 0; count < V - 1; count++) {
        // choose the minimum distance vertex from the set of
        // vertices that are not touched.
        int k = this->minDistance(distances, shortestPathTreeVisited, V);

        // mark this chosen vertex as processed
        shortestPathTreeVisited[k] = true;

        // Update the distance value of the adjacent vertices of the chosen vertex.
        for (int v = 0; v < V; v++)

            // Update distances[v] iff is not in shortestPathTreeVisited, and there is an edge from k to v, and
            // total weight of path from src to v through k is smaller than current value of
            // distances[v]
            if (!shortestPathTreeVisited[v] && adjMatrix[k][v] &&
                    adjMatrix[k][v] != INF &&
                distances[k] + adjMatrix[k][v] < distances[v]) {
                distances[v] = distances[k] + adjMatrix[k][v];
                parentVertexArray[v] = k;
            }
    }


    // list that represents the paths to take
    std::list<T> path;


    // printing
//    std::cout << "\n\nFound path:\n";
//    std::cout << vertexList[src].getValue() << " -> " << vertexList[dest].getValue() << ":\n";

    // save the first starting source vertex to the path
    path.push_back(vertexList[src].getValue());

    // call recursive method to construct the path
    this->constructPath(parentVertexArray, dest, path);

    // temp values to store symbols and costs
    std::string symbol1, symbol2;
    double cost = 0;

    // iterate over the list and store the values into the temp variables
    // since pairs are represented like: "S" -> "K"
    // we might have a path list like: "S T R"
    // Thus, the appropriate pairs are: "S" -> "T" and "T" -> "R"
    for (auto it = path.begin(); it != path.end(); ) {
        if (symbol1 == "") {
            symbol1 = *it;
            ++it;
        }
        else if (symbol2 == "") {
            symbol2 = *it;
            ++it;
        }
        else {
            // at this point, we have 2 symbols, so we need to find the cost and put a new currency pair
            // into the list of pairs
            cost = getWeight(symbol1, symbol2);
            pairs.emplace_back(symbol1, symbol2, cost);

            // reset values
            symbol1 = symbol2 = "";
            cost = 0;
            // back track the iterator pointer by 1
            --it;
        }
    }

    // insert the last currency pair from the path list
    if (symbol1 != "" && symbol2 != "") {
        cost = getWeight(symbol1, symbol2);
        pairs.emplace_back(symbol1, symbol2, cost);
    }


    // check if the current set of pairs results in smaller rate than direct conversion
    double totalConvertedPrice = 1; // converting 1 coin
    for (auto& pair: pairs) {
        totalConvertedPrice *= pair.getPrice();
    }

    // direct price from exchanging 'from' coin to 'to' coin
    double directedPrice = getWeight(from, to);

    // if directed price is smaller, return the currency pair directly
    if (totalConvertedPrice > directedPrice) {
        pairs = std::list<CurrencyPair>();
        pairs.emplace_back(from, to, directedPrice);
    }

    return pairs;
}






