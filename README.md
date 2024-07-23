# Quadtree Implementation Project

This project implements a 2D quadtree data structure using Unreal Engine to efficiently manage and query spatial data. The quadtree structure is used to optimize the performance of particle systems by enabling efficient spatial queries and collision detection.

## Overview

The Quadtree Implementation Project aims to provide a robust and efficient way to manage spatial data in a 2D space. The project utilizes the quadtree data structure to partition the space, enabling quick insertion, deletion, and querying of points. This approach significantly improves the performance of operations like collision detection in particle systems.

## Classes

### AQuadtreeActor

The `AQuadtreeActor` class represents the root of the quadtree. It manages the initialization and updates of the quadtree, as well as handling the insertion and querying of points.

#### Key Properties
- **Root**: The root node of the quadtree.
- **Size**: The size of the area managed by the quadtree.
- **StartPoints**: Initial number of points to insert into the quadtree.
- **Spread**: The spread of points around a center.

#### Key Methods
- `BeginPlay()`: Initializes the quadtree when the game starts.
- `Tick(float DeltaTime)`: Updates the quadtree every frame.
- `InsertPoint(FVector Point)`: Inserts a point into the quadtree.
- `ClearQuadtree()`: Clears the quadtree.
- `InsertRandomPoints(int32 NumberOfPoints)`: Inserts a specified number of random points into the quadtree.
- `QueryPoints(FVector RangeCenter, float RangeSize)`: Queries points within a specified range.

### AQuadtreeNode

The `AQuadtreeNode` class represents a single node within the quadtree. It manages the insertion and querying of points within its region, and can subdivide itself into four child nodes if necessary.

#### Key Methods
- `Initialize(FVector InCenter, float InSize)`: Initializes the node with a center and size.
- `Clear()`: Clears the node and its children.
- `Subdivide()`: Subdivides the node into four child nodes.
- `Insert(FVector Point)`: Inserts a point into the node or its children.
- `Query(FVector RangeCenter, float RangeSize)`: Queries points within a specified range.
- `ContainsPoint(FVector Point)`: Checks if the node contains a specified point.
- `Intersect(FVector RangeCenter, float RangeSize)`: Checks if the node intersects with a specified range.

## Features

- **Efficient Spatial Queries**: Uses a quadtree structure to optimize spatial queries and collision detection.
- **Dynamic Subdivision**: Nodes dynamically subdivide based on the density of points.
- **Real-time Visualization**: Debug visualizations show quadtree partitions and queried points.
- **Performance Metrics**: Measures and compares performance with and without quadtree optimization.

## Credits

This project is inspired by and based on the quadtree data structure discussed in the following resource:
- [Quadtree - Wikipedia](https://en.wikipedia.org/wiki/Quadtree)
![image](https://github.com/user-attachments/assets/b92da2a9-70c0-4499-8f28-b2edb2931b26)
![image](https://github.com/user-attachments/assets/c2808066-0890-4a0b-95cc-8e1120a93530)
![image](https://github.com/user-attachments/assets/f2cbbf63-d45b-4738-a62c-c6977b49cd13)
![image](https://github.com/user-attachments/assets/abdeb7c5-9be7-47fe-a0f2-1020c9fac7fb)



