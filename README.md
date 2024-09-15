**Project: Kandinsky_API**

This project is an experiment aimed at creating API requests to an open GAN (developed by the "Fusion Brain" team).

The project pursues one goal: API requests to the Kandinsky generative network.
1. You automatically receive an up-to-date list of styles upon launch.
2. You can change the style and size of the image.
3. You can provide both positive and negative prompts.

To run the project, you need to register on fusion.ai, obtain a key and secret key, and input them into the Imagegenerator in the specified variables.

The project consists of two key files: Imagegenerator and widget.
Imagegenerator describes a class responsible for forming API requests and processing responses, while widget contains an interface through which users can input data for the requests.

The project is currently in development. The first part, which involves configuring the requests, has been completed, and work is now underway to develop a simple and user-friendly interface. Due to the capabilities of Qt's signals and slots, multiple requests can be sent from a single token.

In the near future, a script will be added to track all sent requests (support for multiple windows for images).
The final product will be a small application that can be integrated into a larger project.

The project is built on the Qt framework.

---

If you need any more assistance, feel free to ask!
