# ConceptNet Python
Python tools to query ConceptNet assertions and NumberBatch embeddings.

## Setup

Clone repo and enter directory:
```
git clone git@github.com:sawyermade/cnet.git; cd cnet
```

Install Anaconda Environment
```
conda env create -f environment.yml
conda activate cnet
```

Get ConceptNet assertions and NumberBatch embeddings, test, and pickle:

```
python3 get_data.py
```

## Epic Kitchens Verb & Noun Paths


## How to use ```conceptnet.py```

Import class and load assertions+embeddings:

```
from conceptnet import ConceptNet
cnet = ConceptNet()
```

Get embedding for a specific concept:

```
embedding = cnet.get_embedding('headset')
```

Get assertions for a specific concept (including assertions originating from other concepts -- *ConceptNet assertions are directed*):

```
assertions = cnet.query_concept('headset')
```

Get assertions originating from a specific concept to any other concept:

```
print(cnet.query_concept('headset', directed=True))
```

Get all assertions between two concepts (in any direction):

```
print(cnet.query_edge('hand', 'arm'))
```

Get all assertions from one concept to another concept:

```
print(cnet.query_edge('hand', 'arm', directed=True))
print(cnet.query_edge('arm', 'hand', directed=True))
```
