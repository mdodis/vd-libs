import xml.etree.ElementTree as ET
import re

def reconstruct_param(param_elem, idx):
    """
    Build a parameter declaration like 'const GLfloat *v'
    Handles spacing, pointers, and unnamed parameters.
    """
    name_node = param_elem.find("name")
    pname = name_node.text.strip() if name_node is not None else f"p{idx}"

    # Gather text around the <name>
    before_name = ""
    if param_elem.text:
        before_name += param_elem.text

    for child in param_elem:
        if child.tag == "ptype":
            before_name += (child.text or "")
            if child.tail:
                before_name += child.tail
        elif child.tag == "name":
            # skip its text
            if child.tail:
                before_name += child.tail
        else:
            before_name += "".join(child.itertext())

    ptype = before_name.strip()

    # --- spacing fixes ---
    # Fix 'constGLint' → 'const GLint'
    ptype = re.sub(r"\bconst(?=[A-Z])", "const ", ptype)
    # Normalize multiple spaces
    ptype = re.sub(r"\s+", " ", ptype)
    # Normalize pointer spacing
    ptype = re.sub(r"(\w)\s*\*", r"\1 *", ptype)
    ptype = re.sub(r"\*\s+", "*", ptype)

    return ptype.strip(), pname


def reconstruct_proto(proto_elem):
    """
    Build (return_type, name) from a <proto> element.
    """
    name_node = proto_elem.find("name")
    if name_node is None:
        return None, None

    retval = ""
    if proto_elem.text:
        retval += proto_elem.text

    for child in proto_elem:
        if child.tag == "ptype":
            retval += (child.text or "")
            if child.tail:
                retval += child.tail
        elif child.tag == "name":
            # skip its text
            if child.tail:
                retval += child.tail
        else:
            retval += "".join(child.itertext())

    retval = retval.strip()
    retval = re.sub(r"\bconst(?=[A-Z])", "const ", retval)
    retval = re.sub(r"\s+", " ", retval)
    retval = re.sub(r"(\w)\s*\*", r"\1 *", retval)
    retval = re.sub(r"\*\s+", "*", retval)

    name = name_node.text.strip()
    if retval.endswith(name):
        retval = retval[: -len(name)].strip()

    return retval, name


def version_key(v):
    try:
        return tuple(map(int, v.split(".")))
    except Exception:
        return (999,)


def generate(glxml_path="gl.xml", backslash=False):
    tree = ET.parse(glxml_path)
    root = tree.getroot()

    # Build command dictionary
    commands = {}
    for cmd in root.findall("commands/command"):
        proto = cmd.find("proto")
        if proto is None:
            continue
        retval, name = reconstruct_proto(proto)
        if not name:
            continue
        params = []
        for i, param in enumerate(cmd.findall("param")):
            ptype, pname = reconstruct_param(param, i)
            params.append((ptype, pname))
        commands[name] = (retval, params)

    # Map version → list of commands (but avoid duplicates)
    version_cmds = {}
    for feature in root.findall("feature"):
        version = feature.get("number")
        if not version:
            continue
        for req in feature.findall("require/command"):
            name = req.get("name")
            if name in commands:
                version_cmds.setdefault(version, set()).add(name)

    # Track commands that have already been output
    seen = set()

    # Output in sorted version order
    for version in sorted(version_cmds.keys(), key=version_key):
        vlabel = version.replace(".", "_")
        print(f"V({vlabel}) \\")

        cmds = sorted(version_cmds[version] - seen)
        for name in cmds:
            retval, params = commands[name]
            param_str = ", ".join(f"{ptype} {pname}" for ptype, pname in params)
            line = f"X({retval}, {name}, ({param_str}))"
            if backslash:
                line += " \\"
            print(line)

        # Mark these as seen so they won’t reappear in later versions
        seen.update(cmds)
        print(f"VE() \\")


if __name__ == "__main__":
    generate("gl.xml", backslash=True)
