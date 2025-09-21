def compare(key_input, list_output, tree_output, title):
    total_suburbs = -1;
    
    key_len = []
    # Create key_len list from input file
    with open(key_input, 'r') as file:
        for line in file:
            key_len.append(len(line.strip())+1)
    
    # Create bits compared and nodes accessed list for linked list    
    list_bits_compared = []
    list_nodes_accessed = []
    with open(list_output, 'r') as file:
        for line in file:
            list_bits_compared.append(int(re.search(r'b(\d+)', line).group(1)))
            list_nodes_accessed.append(int(re.search(r'n(\d+)', line).group(1)))
            if total_suburbs == -1:
                total_suburbs = int(re.search(r's(\d+)', line).group(1))
    
    # Create bits compared and nodes accessed list for patricia tree    
    tree_bits_compared = []
    tree_nodes_accessed = []
    with open(tree_output, 'r') as file:
        for line in file:
            tree_bits_compared.append(int(re.search(r'b(\d+)', line).group(1)))
            tree_nodes_accessed.append(int(re.search(r'n(\d+)', line).group(1)))
    
    # Create dataframe
    df = pd.DataFrame({
        'List Bits Compared': list_bits_compared,
        'Tree Bits Compared': tree_bits_compared,
        'List Nodes Accessed': list_nodes_accessed,
        'Tree Nodes Accessed': tree_nodes_accessed
    }, index=key_len)
    
    df = df.rename_axis('Keys length')
    df = df.groupby("Keys length").mean()
    
    fig, axs = plt.subplots(nrows=1, ncols=2, figsize=(12, 6))

    # Plot the bits compared line chart
    axs[0].plot(df.index, df['List Bits Compared'], marker='o', color='blue', label='Linked List')
    axs[0].plot(df.index, df['Tree Bits Compared'], marker='o', color='red', label='Patricia Tree')
    axs[0].set_title(title)
    axs[0].set_xlabel('Keys length')
    axs[0].set_ylabel('Bits Compared')
    axs[0].legend()

    # Plot the nodes accessed line chart
    axs[1].plot(df.index, df['List Nodes Accessed'], marker='o', color='blue', label='Linked List')
    axs[1].plot(df.index, df['Tree Nodes Accessed'], marker='o', color='red', label='Patricia Tree')
    axs[1].set_title(title)
    axs[1].set_xlabel('Keys Length')
    axs[1].set_ylabel('Nodes Accessed')
    axs[1].legend()

    # Adjust layout
    plt.tight_layout()

    # Show the plot
    plt.show()
    
    # Print summary statistics
    print(f"Suburbs: {total_suburbs}; Keys: {len(key_len)}")
    print()
    
    print("Bits Compared:")
    print(f'List: mean: {round(average(list_bits_compared), 1)}, max: {max(list_bits_compared)}')
    print(f'Tree: mean: {round(average(tree_bits_compared), 1)}, max: {max(tree_bits_compared)}')
    print(f'Average List/Tree = {round(average(list_bits_compared)/average(tree_bits_compared), 1)}')
    print()
    
    print("Average Node Accessed:")
    print(f'List: mean: {round(average(list_nodes_accessed), 1)}, max: {max(list_nodes_accessed)}')
    print(f'Tree: mean: {round(average(tree_nodes_accessed), 1)}, max: {max(tree_nodes_accessed)}')
    print(f'Mean List/Tree = {round(average(list_nodes_accessed)/ average(tree_nodes_accessed), 1)}')
    

def average(lst):
    # Return average of a list
    return sum(lst)/len(lst)
