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

    def emit_command(name):
        retval, params = commands[name]
        param_str = ", ".join(
            f"{ptype} {pname}" for ptype, pname in params
        )
        line = f"X({retval}, {name.removeprefix('gl')}, ({param_str}))"
        if backslash:
            line += " \\"
        print(line)

    # Core versions
    version_cmds = {}
    for feature in root.findall("feature"):
        version = feature.get("number")
        if not version:
            continue

        for req in feature.findall("require"):
            for cmd in req.findall("command"):
                name = cmd.get("name")
                if name in commands:
                    version_cmds.setdefault(version, set()).add(name)

    seen = set()

    for version in sorted(version_cmds, key=version_key):
        vlabel = version.replace(".", "_")
        print(f"VER_START({vlabel}) \\")

        cmds = sorted(version_cmds[version] - seen)
        for name in cmds:
            emit_command(name)

        seen.update(cmds)
        print(f"VER_END({vlabel}) \\")

    # Extensions
    extensions = root.find("extensions")
    if extensions is None:
        return

    for extension in sorted(
        extensions.findall("extension"),
        key=lambda e: e.get("name", "")
    ):
        ext_name = extension.get("name")
        if not ext_name:
            continue

        ext_cmds = set()

        for req in extension.findall("require"):
            for cmd in req.findall("command"):
                name = cmd.get("name")
                if name in commands:
                    ext_cmds.add(name)

        # Skip extensions without commands
        if not ext_cmds:
            continue

        print(f"EXT_START(\"{ext_name}\") \\")

        # Skip commands already emitted by core versions
        # or earlier extensions.
        for name in sorted(ext_cmds - seen):
            emit_command(name)

        seen.update(ext_cmds)
        print("EXT_END() \\")


if __name__ == "__main__":
    generate("gl.xml", backslash=True)
