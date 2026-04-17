# VS1984 Ai Indexer Overview
*Currently, only base on Ubuntu 24.04 and nvidia gpu.*

[English](README.md) | [中文](README.zh-CN.md)

## Overview

This project is a core component example of the **VS1984 Private Knowledge Network**:

👉 **Indexer Service Demo**

It is used for:

- Discovering data providers in a decentralized network

- Indexing and scoring the data capabilities of providers (QScore)

- Providing a **list of candidate data sources** for query nodes

- Supporting the **distributed RAG (Retrieval-Augmented Generation) process**

------

## Architecture Position

In the VS1984 RAG network, there are three core roles:

### 1️⃣ Provider Node

- Holds private data (local knowledge base)

- Runs RAG/LLM locally

- Does not expose raw data, only returns results

### 2️⃣ Indexer Node (This Project)

- Does not store data

- No model inference performed

- Responsibilities:

- Data capability indexing (who has what data)

- Ranking and recommendation (who is more reliable)

- Query routing entry point

👉 Can be understood as:

**Decentralized version of "search engine + recommendation system"**

### 3️⃣ Query Node

- Initiate a query

- Obtain candidate Providers from Indexer

- Request results from Providers

- Pay on demand (on-chain credit)

------

## Core Value

The existence of Indexer solves a key problem:

> Without a central server, **how to find "who has the data you want"?**

## Preparing the Model:

Go to Hugging Face (search for gemma-3-1b-it-Q4_K_M.gguf; other models require you to configure vshome/cnf/config.xbc). Common repositories are *-GGUF repositories (such as bartowski/... or unsloth/...).

Download to your local machine: ./models

```

## Configuration

```text

In the config.xbc configuration file:

"rag": {"indexer": true, "model": "models/gemma-3-1b-it-Q4_K_M.gguf", "rag_path" : "."},

"indexer": true indicates that this node is an Indexer node.

"model": "model_path" is the path to your local LLM model.

"rag_path": "." indicates the path to the rag subroutine.

You can add -dd when starting the program to view more detailed logs. Refer to the VS1984 documentation for details.

```

## Initial Run

```text Generate certificate
`cmd cert <passwd>

After restart
`cmd register

After registration
`cmd bind indexer

This VS1984 node can be registered as an Indexer node

```

# VS1984 Overview

**VS1984** is a **Decentralized Anonymous Communication & Content Sharing Network**.

It aims to provide a self-hostable, anonymous and censorship-resistant
infrastructure for creators and everyday users, in a world that often feels
increasingly like *1984*.

## Core Goals

- **Decentralization** – No central server. Content propagates directly between
peers over a P2P network.
- **Anonymity & Privacy** – A dual identity model (Guard ID / Real ID) that
separates routing identity from settlement identity at the protocol level.
- **End-to-End Encryption** – Content is encrypted by default. Only parties
holding the proper keys can decrypt it.
- **Censorship Resistance** – Multi-hop routing, on-chain proofs and
distributed nodes make takedowns harder and more visible.

## Why VS1984?
Because the world needs a system that does not rely on servers but still provides complete encrypted communication + anonymous content publishing + anonymous transactions.

**VS1984** is designed for scenarios such as:

- ✔ Anonymous chat
- ✔ Anonymous voice calls
- ✔ Uncensorable content publishing
- ✔ Untraceable on-chain transactions
- ✔ Paid BT content platforms
- ✔ A globally distributed, secure communication network
- ✔ An encrypted ecosystem beyond the reach of nation-state censors
When traditional platforms shut down, censorship rises, and communication is restricted, VS1984 can still operate.
