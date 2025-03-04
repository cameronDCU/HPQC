import numpy as np

workspace = np.array([[1.]])  # Initialize workspace
namestack = []  # Track qubit names

def pushQubit(name, weights):
    """Push a new qubit onto the stack, doubling the size of the workspace."""
    global workspace, namestack
    if workspace.shape == (1, 1):  # If workspace is empty
        namestack = []  # Reset stack
    namestack.append(name)  # Push name
    weights = np.array(weights, dtype=workspace[0, 0].dtype) / np.linalg.norm(weights)  # Normalize
    workspace = np.reshape(workspace, (1, -1))  # Convert to row vector
    workspace = np.kron(workspace, weights)  # Apply tensor product

def applyGate(gate, *names):
    """Apply a gate to the specified qubits."""
    global workspace
    for name in names:
        tosQubit(name)
        workspace = np.reshape(workspace, (-1, gate.shape[0]))
        np.matmul(workspace, gate.T, out=workspace)

def tosQubit(name):
    """Move one qubit to the top of the stack and shift others down."""
    global workspace, namestack
    k = len(namestack) - namestack.index(name)  # Find qubit position
    if k > 1:  # If non-trivial
        namestack.append(namestack.pop(-k))  # Rotate name stack
        workspace = np.reshape(workspace, (-1, 2, 2 ** (k - 1)))
        workspace = np.swapaxes(workspace, -2, -1)

def probQubit(name):
    """Calculate total probability of a qubit."""
    global workspace
    tosQubit(name)
    workspace = np.reshape(workspace, (-1, 2))
    prob = np.linalg.norm(workspace, axis=0) ** 2
    return prob / prob.sum()

def measureQubit(name):
    """Measure a qubit and collapse the state."""
    global workspace, namestack
    prob = probQubit(name)
    measurement = np.random.choice(2, p=prob)
    workspace = workspace[:, [measurement]] / np.sqrt(prob[measurement])
    namestack.pop()
    return str(measurement)

# Define quantum gates
X_gate = np.array([[0, 1], [1, 0]])  # Pauli X (NOT)
Y_gate = np.array([[0, -1j], [1j, 0]])  # Pauli Y
Z_gate = np.array([[1, 0], [0, -1]])  # Pauli Z
H_gate = np.array([[1, 1], [1, -1]]) * np.sqrt(1 / 2)  # Hadamard
S_gate = np.array([[1, 0], [0, 1j]])  # Phase gate
T_gate = np.array([[1, 0], [0, np.exp(-1j * np.pi / 4)]])  # T gate
Tinv_gate = np.array([[1, 0], [0, np.exp(1j * np.pi / 4)]])  # T inverse

CNOT_gate = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 0, 1], [0, 0, 1, 0]])  # CNOT
tCZ_gate = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, -1]])  # Controlled-Z
SWAP_gate = np.array([[1, 0, 0, 0], [0, 0, 1, 0], [0, 1, 0, 0], [0, 0, 0, 1]])  # Swap gate

# Rotational and phase gates
def P_gate(phi):
    return np.array([[1, 0], [0, np.exp(1j * phi)]])

def Rx_gate(theta):
    return np.array([[np.cos(theta / 2), -1j * np.sin(theta / 2)],
                     [-1j * np.sin(theta / 2), np.cos(theta / 2)]])

def Ry_gate(theta):
    return np.array([[np.cos(theta / 2), -np.sin(theta / 2)],
                     [np.sin(theta / 2), np.cos(theta / 2)]])

def Rz_gate(theta):
    return np.array([[np.exp(-1j * theta / 2), 0],
                     [0, np.exp(1j * theta / 2)]])

# Simple usage example
pushQubit("Q1", [1, 0])
applyGate(H_gate, "Q1")
print("Q1 probabilities:", probQubit("Q1"))  # Peek Q1 prob
pushQubit("Q2", [0.6, 0.8])
print("Q2 probabilities:", probQubit("Q2"))  # Peek Q2 prob
print(measureQubit("Q1"), measureQubit("Q2"))
