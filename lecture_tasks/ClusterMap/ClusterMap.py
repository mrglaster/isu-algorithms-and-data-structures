import json
import shapely
import math
import matplotlib.pyplot as plt
import heapq
import networkx as nx
import time

INPUT_DATA_FILE = 'json/base.json'

# SOURCE: https://gist.github.com/hrbrmstr/91ea5cc9474286c72838
GEO_JSON_FILE = 'json/mapdata.json'
EARTH_RADIUS = 6371
NA_MAX_DISTANCE = 7327


def get_spanning_tree_custom(graph: nx.Graph()) -> nx.Graph():
    """Spanning tree getting function using heapq"""
    edges = []
    tree = nx.Graph()
    start_node = list(graph.nodes)[0]
    visited = {start_node, }
    connected_edges = [(u, v, graph[u][v]["weight"]) for u, v in graph.edges(start_node)]
    heapq.heapify(connected_edges)
    while connected_edges:
        u, v, weight = heapq.heappop(connected_edges)
        if v not in visited:
            edges.append((u, v, weight))
            visited.add(v)
            for next_u, next_v in graph.edges(v):
                if next_v not in visited:
                    heapq.heappush(connected_edges, (next_u, next_v, graph[next_u][next_v]["weight"]))
    tree.add_weighted_edges_from(edges)
    return tree


def generate_graphdata_fsites(sites: dict, max_distance=400):
    edges = []
    nodes = []
    for i, site in enumerate(sites[:-1]):
        nodes.append(site["code"])
        min_distance = NA_MAX_DISTANCE
        min_dest = None
        min_dest_flag = True
        for dest in sites[i + 1:]:
            d = get_distance(site, dest)
            if d <= max_distance:
                edges.append((site["code"], dest["code"], d))
                min_dest_flag = False
            if d < min_distance:
                min_dest = dest
                min_distance = d
        if min_dest_flag:
            edges.append((site["code"], dest["code"], min_distance))
    return edges, nodes


def get_clusters(spanning_tree: nx.Graph, n_clusters: int) -> list:
    """Splits the tree into n_clusters clusters"""
    sorted_edges = sorted(spanning_tree.edges(data=True), key=lambda x: x[2]['weight'], reverse=True)
    copy_tree = spanning_tree.copy()
    for i in range(n_clusters - 1):
        if len(sorted_edges) > i:
            copy_tree.remove_edge(*sorted_edges[i][:2])
    return list(nx.connected_components(copy_tree))


def get_distance(site1: dict, site2: dict) -> float:
    """Calculates distance between 2 points by their latitude and longitude using the haversine formula"""
    loc1 = site1["location"]
    loc2 = site2["location"]
    lat1, lat2 = math.radians(loc1["lat"]), math.radians(loc2["lat"])
    dlat, dlon = math.radians(loc2["lat"] - loc1["lat"]), math.radians(loc2["lon"] - loc1["lon"])
    a = math.sin(dlat / 2) ** 2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    distance = EARTH_RADIUS * c
    return round(distance, 2)


def _get_pure_north_america(polygon) -> []:
    """Finds the largest polygon via vector multiplication. The source data contains not only North America but also Greenland and some ither stuff"""
    max_area = 0
    max_poly = []
    for i in range(len(polygon)):
        points = polygon[i][0]
        area = shapely.Polygon(points).area
        if area > max_area:
            max_area = area
            max_poly = points
    return max_poly


def get_north_america() -> shapely.Polygon:
    """Returns Polygon of for  North America"""
    global GEO_JSON_FILE
    with open(GEO_JSON_FILE, "r") as file:
        continents = json.load(file)
        north_america_polygon = continents["features"][1]["geometry"]["coordinates"]
    return shapely.Polygon(_get_pure_north_america(north_america_polygon))


def get_origins():
    """Parses origins from JSON file """
    global INPUT_DATA_FILE
    with open(INPUT_DATA_FILE, "r") as f:
        sites = json.load(f)
        lons = []
        lats = []
        for site in sites:
            lons.append(site["location"]["lon"])
            lats.append(site["location"]["lat"])
        return lons, lats, sites


def get_north_america_sites():
    """Returns dictionary of North America Sites"""
    polygon = get_north_america()
    north_america_sites = []
    res = 0
    lons, lats, sites = get_origins()
    for site in sites:
        p = shapely.Point((site["location"]["lon"], site["location"]["lat"]))
        if polygon.contains(p):
            res += 1
            north_america_sites.append(site)
    return north_america_sites


def process_clustering(clusters_amount: int):
    """Splits North American sites to N clusters"""
    limiter = 100
    sites = get_north_america_sites()
    edges, nodes = generate_graphdata_fsites(sites[:limiter])
    start = time.time()
    graph = nx.Graph()
    graph.add_nodes_from(nodes)
    graph.add_weighted_edges_from(edges)

    spanning_tree = get_spanning_tree_custom(graph)
    clusters = get_clusters(spanning_tree, clusters_amount)
    print(len(clusters))
    end = time.time()
    print(f"Calculation Time: {end - start} for {limiter} sites")


def process_file():
    """Parses points from the file, draws'em all and colors points in North America"""
    flons = []
    flats = []
    for site in north_america_sites:
        flons.append(site["location"]["lon"])
        flats.append(site["location"]["lat"])
    plt.scatter(lons, lats)  # All the points
    plt.scatter(flons, flats)  # Points in North America
    plt.show()


def main():
    """The main function"""
    process_clustering(clusters_amount=40)


if __name__ == '__main__':
    """The main function invocation"""
    main()
