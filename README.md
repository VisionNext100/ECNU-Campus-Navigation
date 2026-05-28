# ECNU-Campus-Navigation
## Yehan WANG, ECNU

### Ⅰ Overview
<div align="center">
    <img src="https://cdn.jsdelivr.net/gh/VisionNext100/Image-Hosting/images/ECNU-Campus-Navigation/ECNU_Putuo_Campus_Map.jpg" width="300" alt="ECNU Putuo Campus Map">
    <br>
    <em>ECNU Putuo Campus Map</em>
</div> 

East China Normal University is a renowned comprehensive university located in Shanghai, boasting a long history and a rich campus culture. With the continuous development and expansion of the university campuses, a campus navigation application is essential to help freshmen, visitors, and campus members better familiarize themselves with the environment. This project delivers a robust campus navigation system implemented in C++ to provide a wide array of routing, location management, and graph-theoretic algorithm functionalities.

### Ⅱ Structure
```
ECNU-Campus-Navigation/
├── README.md
├── LICENSE
├── .gitignore
├── CampusNavigation/
│   ├── Algorithm.cpp
│   ├── Algorithm.h
│   ├── CMakeLists.txt
│   ├── GraphException.h
│   ├── LGraph.cpp
│   ├── LGraph.h
│   ├── LocationInfo.h
│   └── main.cpp
└── cases/
    ├── chain_5/
    │   ├── answer.txt
    │   ├── command.txt
    │   ├── edges.csv
    │   └── nodes.csv
    ├── grid_100/
    │   ├── answer.txt
    │   ├── command.txt
    │   ├── edges.csv
    │   └── nodes.csv
    ├── random_dense_100/
    │   ├── answer.txt
    │   ├── command.txt
    │   ├── edges.csv
    │   └── nodes.csv
    ├── ring_100/
    │   ├── answer.txt
    │   ├── command.txt
    │   ├── edges.csv
    │   └── nodes.csv
    └── star_100/
        ├── answer.txt
        ├── command.txt
        ├── edges.csv
        └── nodes.csv   
```

### Ⅲ Implementation
1\. **Vertex Management**  
- Add, delete, modify, and query detailed campus location information, including site name, location type, and recommended visiting duration.
- Display comprehensive summaries of all location information currently stored in the system.
- Support data persistence by importing or exporting vertex datasets from/to external CSV files.  

2\. **Edge Management**  
- Add, delete, modify, and query routing paths connecting different campus locations.
- Display comprehensive summaries of all edge connectivity and distance matrix information.
- Support data persistence by importing or exporting edge datasets from/to external CSV files.  

3\. **Navigation & Graph Algorithms** 
- Compute the absolute shortest path between any two designated locations based on Dijkstra's algorithm.
- Analyze the graph topology to determine the existence of an Eulerian Path based on undirected graph degree conditions.
- Calculate the Minimum Spanning Tree (MST) across the campus graph using Kruskal's algorithm to optimize path coverage.
- Perform topologically constrained shortest path calculations based on specific routing restrictions.
- Query and display all adjacent edges and immediate neighbors of any selected location vertex.
- Filter and query campus locations dynamically based on specific category types.  

4\. **Multi-Campus Cross-Tour Route Planning**
- Plan comprehensive excursion routes specifically from the North Zhongshan Road Campus gate to the Minhang Campus gate. 
- Allow users to input a customized time budget (which must be greater than or equal to the absolute minimum path travel time between the two campus gates).
- Generate all feasible tour schemes within the user's specified time budget, dynamically calculating the total time consumption by summing both path travel times and individual spot-visiting durations.


### Ⅳ Compilation
TODO
