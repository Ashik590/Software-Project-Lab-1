# The El doˈɾaðo Explorer

**Author:** Md. Azizul Hakim Ashik

**Project Supervisor:** Prof. Dr. Mohammand Shoyaib 

---

## Installation and Compilation Guide

### Requirements
- Ubuntu Linux
- g++ (C++ compiler)
- Poppler C++ library
- pkg-config

### Install Dependencies
```bash
sudo apt update
sudo apt install libpoppler-cpp-dev
sudo apt install pkg-config
```

### Train the Bayes Classifier Models

#### Model 1
```bash
g++ Train_models/train_model_1.cpp -o model1
./model1
```

#### Model 2
```bash
g++ Train_models/train_model_2.cpp -o model2
./model2
```

### Compile the Main Application
```bash
g++ main.cpp Implementation/* `pkg-config --cflags --libs poppler-cpp` -o a
```

### Run the Application
```bash
./a
```

---

**Good to know**
- The `Files/` directory contains sample files and images to test the application.
- `@` is the command used inside the application to keep the cache updated and efficient.
- `#` is the command used to completely clear the cache.

---

## About the Project

**The El doˈɾaðo Explorer** is a terminal-based local retrieval system built in **pure C++** for **Linux (Ubuntu)**. It was developed as part of the academic course **Software Project Lab-1** in the **2nd year, 1st semester**.

The project is designed to efficiently search and extract information from large-scale heterogeneous local data sources, especially **PDF documents** and **images**. It uses a **two-phase retrieval strategy** with caching to avoid repeated expensive computation.

---

## Project Highlights

- Terminal-based application
- Runs only on **Linux (Ubuntu)**
- Built using **pure C++**
- Supports **pluggable search modules**
- Uses **cached representations** for faster repeated queries
- Includes:
  - **PDF Text Retrieval**
  - **Bayesian-based Image Skin Region Retrieval**

---

## Retrieval Strategy

The system follows a **two-phase retrieval approach**:

### 1. Initial Query
- Uses direct brute-force processing
- Performs full computation
- Includes PDF parsing and pixel-level analysis

### 2. Subsequent Queries
- Uses cached module-specific representations
- Avoids repeated expensive computations
- Produces significantly faster results

---

## Current Modules

### Module 1: PDF Text Retrieval
This module performs keyword search over PDFs using linear scanning and cached text representations.

**Features**
- Search by keyword in PDF documents
- Supports whole word or partial word matching
- Case-sensitive or case-insensitive search
- Deep search mode with word suggestions
- Navigation to the exact PDF page

**Inputs**
- Search mode: PDF or Image
- Search scope
- Number of roots
- Root names
- Search depth
- Whole or partial word
- Search level
- Keyword mode
- Keywords
- Case sensitivity

**Outputs**
- File paths inside roots
- Page numbers
- Line numbers
- Target lines containing keywords
- Word suggestions

**Navigation**
- The user enters the serial number
- The corresponding PDF opens at the specified page in the browser

**Deep Search Suggestions**
When Deep Search is enabled, the system shows word suggestions using three matching algorithms:
- Levenshtein distance
- Jaro-Winkler similarity
- Soundex

**Why caching matters**
PDF text extraction is expensive, so the application stores extracted text after the first search and reuses it in later searches.

---

### Module 2: Skin Region Retrieval
This module identifies and extracts human skin regions from images using a **pixel-level Naive Bayes classifier**.

**Features**
- Pixel-level human skin region classification
- Caches skin-segmented image representations
- Lets the user choose which image result to view

**Inputs**
- Search mode: PDF or Image
- Model version
- Root name
- Search depth

**Outputs**
- Skin-segmented image results
- Cached image representations for faster reuse

**Why caching matters**
Pixel-level classification is computationally expensive, so the system stores precomputed skin-segmentation results to avoid repeating the same work.

---

## What I Learned From This Project

This project helped me understand how machine learning models are implemented in real software systems. In particular, I learned how to build and apply a **supervised learning model**, especially a **Naive Bayes classifier**, inside a practical application.

---

## Sample Data

The `Files/` directory contains sample files and images that can be used to test the application.

---

## Presentation

The project is fully described in the presentation slides stored in the `presentation/` directory.

---

## Notes

- This application is intended to run on **Linux (Ubuntu)** only.
- It is built using **pure C++**.
- The caching system is a key part of its performance improvement.

---

## License

Copyright (c) 2026 Md. Azizul Hakim Ashik

All Rights Reserved.

This project was developed as part of the academic course
"Software Project Lab-1". Unauthorized copying, modification,
distribution, or commercial use of this software is prohibited
without explicit permission from the author.