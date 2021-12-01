from itertools import islice
import networkx as nx
import matplotlib.pyplot as plt
import random as rand

#currently doesn't save the plot anywhere, just draws it.
def generate_network(nodes):
    G = nx.gnp_random_graph(nodes, .5)
    # assign weights to edges
    for (u, v) in G.edges():
        G[u][v]['weight'] = rand.randint(1, 10)
    pos = nx.spring_layout(G)
    nx.draw(G, pos, with_labels = True)
    edge_labels = dict([((u, v,), d['weight'])
                    for u, v, d in G.edges(data=True)])
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels)
    plt.show()
    return G

#saves the information to output.txt
def save_network(G, k_paths, path = 'output.txt'):
    nx.write_edgelist(G, path)
    file = open(path, 'a')
    file.write(str(k_paths))
    file.close()

#calculates the k shortest paths from node a to node B
def k_shortest_paths(G, start, end, k, weight='weight'):
    return list(islice(nx.shortest_simple_paths(G, start, end, weight=weight), k))


G = generate_network(10)
k_paths = k_shortest_paths(G, 0, 9, 3)
save_network(G, k_paths)