/*
 *      ##################################################
 *            EHB208E - DATA STRUCTURES & PROGRAMMING
 *      --------------------------------------------------
 *                          HOMEWORK - 2
 *      --------------------------------------------------
 *          SHORTEST PATH BETWEEN THE GIVEN TWO CITIES
 *      --------------------------------------------------
 *                       HOW DOES IT WORK?
 *
 *      1 - Takes program arguments when has started.
 *      2 - Argument format:
 *            ./compiled -i input.txt -o output.txt
 *      3 - Input file consists cities and distances.
 *      Also, there are number of cities, number of path.
 *      4 - Last line in input file gives initial city and
 *      destination city.
 *      5 - This program calculates the shortest path
 *      between these 2 cities based on this information.
 *      6 - Finally, it writes shortest path and distance
 *      to output file.
 *
 *      [IMPORTANT]: To calculate the shortest path, this
 *      program uses "Dijkstra Algorithm". And It runs
 *      this algorithm by creating the adjacency matrix.
 *      --------------------------------------------------
 *      Class Lecturer: Academic Dr. Bora Döken
 *      Class Asistant: Res. Asst. Muhammed Yasin Adıyaman
 *
 *      Student and Writer: İsmet Erdoğan
 *      School Number: 040200522
 *      Department: Control and Automation Eng.
 *      ###################################################
 */

//  required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  The data structure that holds the distance from city A to city B, and city names A and B.
struct edge {
  char from_city[100];
  char to_city[100] ;
  int distance;
};

//  Type definition is used to make it easy to use.
typedef struct edge Edge;

//  Function Prototypes
static void show_usage(char* name);
int* dijkstra(const int* adjacency_matrix, int source, int destination, int numberOfNodes);
int* createAdjacencyMatrix(Edge* edgeList, int numberOfNodes, int numberOfEdges, char cities[][10]);
void printMatrix(const int* matrix, int column, int row, char cities[][10]);
int cityNameToIndex(char* name,char cities[][10], int numberOfNodes);
void parseResultAndWriteToFile(const int * resultArray, int numberOfNodes, char cities[][10], char* outputFileName);

//  The constant value named MAXI is defined to indicate positive infinity.
const int MAXI = 99999999;

//  Main Function
int main(int argc, char* argv[])
{
//  This and the following lines are written to separate and read program arguments.
	if(argc<5){
		show_usage(argv[0]);
		return 2;
	}
	char inFile[100], outFile[100];
	for (int i = 1; i < argc; ++i) {
        char arg[15];
        strcpy(arg, argv[i]);
        if (!strcmp(arg,"-h") || !strcmp(arg,"--help")) {
            show_usage(argv[0]);
            return 0;
        } else if (!strcmp(arg,"-i") || !strcmp(arg,"--inFile")) {
            if (i + 1 < argc) { 
                strcpy(inFile, argv[++i]);
            } else {
                fprintf(stderr, "--inFile option requires one argument.");
                return 1;
            }
		} else if (!strcmp(arg,"-o") || !strcmp(arg,"--outFile")) {
            if (i + 1 < argc) { 
                strcpy(outFile, argv[++i]);
            } else { 
                fprintf(stderr, "--outFile option requires one argument.");
                return 1;
            }   
        } else {
            show_usage(argv[0]);
			return 1;
        }
    }
//  ----------------------------------------------------------------------------------------
//    The edge distance
	int weight;
//    Source(a) and destination(b) for each edge
	char sourceString[100], destinationString[100];
//    Initial(fromNode) and finalNode(toNode) nodes
	char initialNode[100], finalNode[100];

//    Just temporary variable to hold tokens which are in the same line.
	char tokenHolder[100];

//    The file entered as the input (program argument -i .....) file opens.
    FILE* fp = fopen(inFile, "r");
//    The first line is read.
    fgets(tokenHolder, 100, fp);

//    Number of Edges and Number of Nodes are defined as integer.
//    Then, the values are assigned to the "numberOfEdges" and "numberOfNode" by reading the first line of the file.
	int numberOfEdges, numberOfNodes;
	sscanf(tokenHolder, "%d %d", &numberOfNodes, &numberOfEdges);

//    edges holds fromCity, toCity and distance.
    Edge edges[numberOfEdges];

	//Input file reading.
	//Containers are initialized.
    int edgeCount=0;

    for(int i=0; i < numberOfEdges; i++){
        fgets(tokenHolder,100,fp);
        sscanf(tokenHolder, "%s %s %d", sourceString, destinationString, &weight);
        strcpy(edges[edgeCount].from_city, sourceString);
        strcpy(edges[edgeCount].to_city, destinationString);
        edges[edgeCount].distance = weight;
        edgeCount++;
    }

	//Last line in input file is read.	
	fgets(tokenHolder,100,fp);
	sscanf(tokenHolder, "%s %s", initialNode, finalNode);

//    File closes.
	fclose(fp);


//    printf("%s","Roads [fromCity, toCity, distance]:\n");

//    //Control if input is read correctly
//    for(int i=0; i < numberOfEdges; i++){
//        printf("- %s -> %s: %d\n", edges[i].from_city, edges[i].to_city, edges[i].distance);
//    }

    // Identify unique cities
    char cities[numberOfNodes][10];
    int num_cities = 0;
    for (int i = 0; i < numberOfEdges; i++)
    {
        Edge edge = edges[i];
        int from_city_exists = 0;
        int to_city_exists = 0;
        for (int j = 0; j < num_cities; j++) {
            if (strcmp(cities[j], edge.from_city) == 0) {
                from_city_exists = 1;
            }
            if (strcmp(cities[j], edge.to_city) == 0) {
                to_city_exists = 1;
            }
        }
        if (!from_city_exists) {
            strcpy(cities[num_cities], edge.from_city);
            num_cities++;
        }
        if (!to_city_exists) {
            strcpy(cities[num_cities], edge.to_city);
            num_cities++;
        }
    }

    printf("\n\t\t\t------SHORTEST PATH BETWEEN TWO CITIES------\n\n");
    printf("- There are %d cities in input file.\n", numberOfNodes);
    printf("- There are %d roads in input file.\n", numberOfEdges);
    printf("- Initial city is determined as %s in input file.\n", initialNode);
    printf("- Destination city is determined as %s in input file.\n", finalNode);

    int* adjMatrix = createAdjacencyMatrix(edges, numberOfNodes, numberOfEdges, cities);
    printMatrix(adjMatrix, numberOfNodes, numberOfNodes, cities);

//    printf("%s", "\n\nSource city:");
//    scanf("%s", initialNode);
//    printf("%s", "Destination city:");
//    scanf("%s", finalNode);

    int initialCityIndex = cityNameToIndex(initialNode, cities, numberOfNodes);
    int finalCityIndex = cityNameToIndex(finalNode, cities, numberOfNodes);


    int *results = dijkstra(adjMatrix, initialCityIndex,
                            finalCityIndex, numberOfNodes);

//    for (int i = 0; i < numberOfNodes + 1; ++i) {
//        printf("%d    ", results[i]);
//    }

    parseResultAndWriteToFile(results, numberOfNodes, cities, outFile);
	return 0;
}
//  End of main() Function

//  Function Definitons
int* dijkstra(const int* adjacency_matrix, int source, int destination, int numberOfNodes) {
    /*
     *          - dijkstra(adjacency_matrix, source, destination, numberOfNodes) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> This function uses to find the shortest path between two nodes (applies
     *          Dijkstra Algorithm).
     *          -> Gets adjacency matrix, source and destination.
     *          -> Finds shortest path and distance.
     *          -> Returns array contains distance, visited cities and stop value.
     *
     *          [Return Format]: {Distance, Source, Visited City1,..., Destination City,
     *          STOP VALUE}
     */

    // distance array to store the shortest distance from source to each vertex
    int distance[numberOfNodes];

    // predecessor array to store the predecessor of each vertex in the shortest path
    int predecessor[numberOfNodes];


    // boolean array to track which
    // initialize distance and predecessor arrays
    int visited[numberOfNodes];

    for (int i = 0; i < numberOfNodes; i++) {
        // set all distances to infinity
        distance[i] = MAXI;
        // set all predecessors to -1
        predecessor[i] = -1;
        // mark all vertices as unvisited
        visited[i] = 0;
    }
    // set distance of source vertex to 0
    distance[source] = 0;

    // loop until all vertices have been visited
    for (int count = 0; count < numberOfNodes - 1; count++) {
        // find the vertex with the minimum distance
        int min = MAXI;
        int min_index;
        for (int v = 0; v < numberOfNodes; v++) {
            if (!visited[v] && distance[v] <= min) {
                min = distance[v];
                min_index = v;
            }
        }
        // mark the vertex as visited
        visited[min_index] = 1;

        // update the distance of the adjacent vertices
        for (int v = 0; v < numberOfNodes; v++) {
            // if v is not visited and there is an edge from min_index to v
            if (!visited[v] && *(adjacency_matrix + min_index * numberOfNodes + v) != MAXI && distance[min_index] !=
            MAXI && distance[min_index] + *(adjacency_matrix + min_index * numberOfNodes + v) < distance[v]) {
                distance[v] = distance[min_index] + *(adjacency_matrix + min_index * numberOfNodes + v);
                predecessor[v] = min_index;
            }
        }
    }

    int* resultArray = calloc(numberOfNodes + 2, sizeof(int));
    int pointerShifter = 1;

//    printf("Path: ");
    int path[numberOfNodes];
    int path_index = 0;
    int current = destination;
    while (current != -1) {
        path[path_index++] = current;
        current = predecessor[current];
    }
    // reverse the path
    for (int i = path_index - 1; i >= 0; i--) {
        if (i == path_index - 1) {
//            printf("%s ", cities[path[i]]);
            resultArray[pointerShifter] = path[i];
        }
        else {
//            printf("-> %s ", cities[path[i]]);
            pointerShifter++;
            resultArray[pointerShifter] = path[i];
        }
    }
    // print the shortest distance from source to destination
//    printf("\nDistance: %d km\n", distance[destination]);
//    printf("\n");

    resultArray[0] = distance[destination];
    resultArray[++pointerShifter] = 99;

//    return distances and visited cities to use in main() function.
    return resultArray;
}
int* createAdjacencyMatrix(Edge* edgeList, int numberOfNodes, int numberOfEdges, char cities[][10]) {
    /*
     *          - createAdjacencyMatrix(Edge* edgeList, numberOfNodes, numberOfEdges, cities[][]) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> This function creates adjacency matrix from edge list (consists A, B cites and distance
     *          between these cities.)
     *          -> Firstly, creates numberOfNodes * numberOfNodes matrix. And, assigns all terms to infinity.
     *          -> Then, navigates edgeList; and, assign distances between cities which has adjacency rela..
     *          ..tionship.
     */
    int *matrix = calloc(numberOfNodes * numberOfNodes, sizeof(int));

    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; ++j) {
            *(matrix + (i * numberOfNodes) + j) = MAXI;
        }
    }

    // Add distances to the matrix
    for (int i = 0; i < numberOfEdges; i++) {
        Edge edge = edgeList[i];
        int from_index = -1;
        int to_index = -1;
        for (int j = 0; j < numberOfNodes; j++) {
            if (strcmp(cities[j], edge.from_city) == 0) {
                from_index = j;
            }
            if (strcmp(cities[j], edge.to_city) == 0) {
                to_index = j;
            }
        }
        *(matrix + from_index * numberOfNodes + to_index) = edge.distance;
    }

    return matrix;
}
void printMatrix(const int* matrix, int column, int row, char cities[][10]) {
    /*
     *          - printMatrix(matrix, column, row, cities[][]) Function -
     *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     *          -> Print matrix in matrix form. (column and size form)
     *          -> It uses the cities[][] array to make the matrix more
     *          meaningful and prints the city names on the matrix.
     */
    printf("%s","\n\t\t\t\t   ---ADJACENCY MATRIX---\n\n");
    printf("%10s ", "");
    for (int i = 0; i < row; i++)
    {
        printf("%10s ", cities[i]);
    }
    printf("%s","\n\n");
    for (int i = 0; i < column; i++) {
        printf("%10s ", cities[i]);
        for (int j = 0; j < row; ++j) {
            if (*(matrix + (i * column) + j) == MAXI) {
                printf("%10s ", "inf");
            }
            else {
                printf("%10d ", *(matrix + (i * column) + j));
            }
        }
        printf("%s","\n");
    }

}
int cityNameToIndex(char* name,char cities[][10], int numberOfNodes) {
/*
 *          - cityNameToIndex(city name, cities[][], numberOfNodes) Function -
 *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *          -> It finds city in cities.
 *          -> And, returns index value.
 */
    int index;
    int isExist = 0;
    for (int i = 0; i < numberOfNodes; i++)
    {
        if ((strcmp(name, cities[i]) == 0) && isExist == 0)
        {
            index = i;
            isExist = 1;
        }
    }
    if (!isExist)
    {
        printf("[ERROR]: There is no such city name which named %s", name);
        exit(1);
    }
    return index;
}
void parseResultAndWriteToFile(const int * resultArray, int numberOfNodes, char cities[][10], char* outputFileName) {
/*
 *          - parseResultAndWriteToFile(resultArray, numberOfNodes, cities[][], outputFileName) Function -
 *          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *          -> resultArray is the value returned by the dijkstra(...) function.
 *          -> This function first splits the resultArray according to its own format and extracts the
 *          meaningful information.
 *          -> These information are visited cities and total distance.
 *          -> [Format of resultArray]: {Distance, Source, Visited City1,..., Destination City, STOP VALUE}
 *          -> Then, writes to the .txt file which entered function argument named outputFileName.
 *          -> outputFileName argument holds output file's name which entered as a program argument.
 *          -> Also, it prints cities and total distance to terminal
 *
 */
    FILE *fptr;
    fptr = fopen(outputFileName, "w");

    int stop_value = 99;
    int distance;
    distance = resultArray[0];
    printf("\n\nPath: ");
    fprintf(fptr,"%s ", "Path:");
    for (int i = 1; i < numberOfNodes; i++)
    {
        if (*(resultArray + i) == stop_value)
        {
            break;
        }
        else if (i == 1)
        {
            printf("%s", cities[*(resultArray + i)]);
            fprintf(fptr,"%s", cities[*(resultArray + i)]);
        }
        else if (i > 1)
        {
            printf(" -> %s", cities[*(resultArray + i)]);
            fprintf(fptr," -> %s", cities[*(resultArray + i)]);
        }

    }
    printf("\nDistance: %d km\n", distance);
    fprintf(fptr,"\nDistance: %d km", distance);

    fclose(fptr);
}
static void show_usage(char* name)
{
    fprintf(stderr, "Usage: %s <option(s)> SOURCES\n\
\tOptions:\n\
\t\t-h,--help\t\tShow this help message\n\
\t\t-i,--infile INFILE\tSpecify the input file path\n\
\t\t-o,--outfile OUTFILE\tSpecify the output file path\n", name);
}
