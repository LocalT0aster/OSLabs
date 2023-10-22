import json
import subprocess


def create_markdown_tree(node, path=""):
    # Base case: if it's a file, return its markdown link
    if node["type"] == "file":
        filename = node["name"]
        filepath = path + filename
        return f"- [{filename}]({filepath})\n"

    # For directories, we'll create a dropdown
    elif node["type"] == "directory":
        dir_name = node["name"]
        new_path = path + dir_name + "/"
        content = [f"<details>\n<summary>{dir_name}</summary>\n\n"]

        # Recursively process all children (files and sub-directories)
        for child in node["contents"]:
            content.append(create_markdown_tree(child, new_path))
        
        content.append("\n</details>\n")
        return "".join(content)

    return ""

def main():
    # Fetch the JSON representation of the directory using tree command
    result = subprocess.run(['tree', '-J'], capture_output=True, text=True)
    data = json.loads(result.stdout)[0]

    markdown_tree = create_markdown_tree(data)
    print(markdown_tree)

if __name__ == "__main__":
    main()
