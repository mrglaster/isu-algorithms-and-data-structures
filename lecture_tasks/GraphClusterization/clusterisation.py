import random
import networkx as nx
from networkx.drawing.nx_pydot import graphviz_layout
import matplotlib.pyplot as plt


def draw_graph(graph: nx.Graph()):
    """Draws Graph"""
    pos = nx.spring_layout(graph)
    plt.figure(figsize=(7, 6))
    nx.draw_networkx_nodes(graph, pos)
    nx.draw_networkx_edges(graph, pos, graph.edges)
    nx.draw_networkx_labels(graph, pos, font_size=20)
    labels = nx.get_edge_attributes(graph, "weight")
    nx.draw_networkx_edge_labels(graph, pos, edge_labels=labels)
    plt.show()


def get_graph_weight(graph: nx.Graph()):
    """Returns weight of the graph: the summary length of all edges"""
    weight = 0
    for edge in graph.edges:
        weight += graph.get_edge_data(*edge)["weight"]
    return weight


def get_minimum_spanning_tree(graph: nx.Graph()):
    """Algorithm finding minimal spanning tree"""

    # Visited  Nodes
    visited = set()

    # selects random node
    start_node = random.choice(list(graph.nodes()))
    visited.add(start_node)

    # Initalize new graph
    minimum_spanning_tree = nx.Graph()

    # Visiting the rest of the nodes
    while len(visited) < len(graph.nodes()):

        # Potential edges for the new graph
        possible_edges = []

        for visited_node in visited:

            # Selecting all neighbours
            for neighbor, attrs in graph[visited_node].items():
                if neighbor not in visited:
                    possible_edges.append((visited_node, neighbor, attrs["weight"]))

        # Getting minimal value
        min_edge = min(possible_edges, key=lambda e: e[2])
        # adding a new adge to the minimum spanning tree
        minimum_spanning_tree.add_edge(min_edge[0], min_edge[1], weight=min_edge[2])

        # Adding the visited node to the array of visited ones
        visited.add(min_edge[1])

    return minimum_spanning_tree


def get_minimum_spanning_tree_short(graph: nx.Graph()):
    """Also an Prim-based algorithm finding the minimum spanning tree """
    edges, nodes = [], []
    for node in graph.nodes:
        nodes.append(node)
        connected_edges = graph.edges(node)
        min_edge = min(connected_edges, key=lambda e: graph.get_edge_data(*e)["weight"])
        edges.append(min_edge + (graph.get_edge_data(*min_edge)["weight"],))
    result = nx.Graph()
    result.add_weighted_edges_from(edges)
    result.add_nodes_from(nodes)
    return result


def generate_demo_graph():
    """Generates demo graph"""
    graph = nx.Graph()
    graph.add_edge("A", "C", weight=2)
    graph.add_edge("A", "B", weight=7)
    graph.add_edge("A", "D", weight=4)
    graph.add_edge("B", "C", weight=3)
    graph.add_edge("B", "D", weight=5)
    graph.add_edge("C", "D", weight=2)
    graph.add_edge("C", "E", weight=6)
    graph.add_edge("D", "E", weight=4)
    return graph


def get_clusters(spanning_tree: nx.Graph, n_clusters: int):
    """Splits the tree into n_clusters clusters"""
    # Sort edges of the spanning_tree
    sorted_edges = sorted(spanning_tree.edges(data=True), key=lambda x: x[2]['weight'], reverse=True)
    copy_tree = spanning_tree.copy()
    for i in range(n_clusters - 1):
        copy_tree.remove_edge(*sorted_edges[i][:2])
    draw_graph(copy_tree)
    return list(nx.connected_components(copy_tree))


def main():
    demo_graph = generate_demo_graph()

    newer_graph = get_minimum_spanning_tree(demo_graph)
    newer_graph_also = get_minimum_spanning_tree_short(demo_graph)

    clusters = get_clusters(newer_graph, 4)
    print(len(clusters))


if __name__ == '__main__':
    main()
