
import os
import pathlib
import re
import sys
import xml.etree.ElementTree as etree
import xml.dom.minidom as minidom

RE_WARN_LOGFILE = re.compile(r"WARN_LOGFILE\s*=\s*(.+)")


def path_to_dotted(path, prefix):
    split_path = [part.replace(".", "_")
                  for part in path.relative_to(prefix.parent).parts]
    return ".".join(split_path[:-1]), split_path[-1]


def build_report(issues, path_prefix):
    tree = etree.ElementTree(etree.Element("testsuite", tests=str(len(issues))))
    test_suite = tree.getroot()
    for issue in issues:
        class_, name = path_to_dotted(issue["path"], path_prefix)
        test_case = etree.Element("testcase", classname=class_, name=name)
        failure = etree.Element("failure", type=issue["type"])
        failure.text = issue["message"]
        test_case.append(failure)
        test_suite.append(test_case)
    return tree


def log_to_junit(log, dest, path_prefix):
    issues = []
    with log.open() as log_fp:
        for line in log_fp:
            try:
                path, lineno, type_, message = line.split(":", 3)
            except ValueError:
                # Message spans multiple lines
                issues[-1]["message"] += line
            else:
                path = pathlib.Path(path)
                lineno = int(lineno)
                type_ = type_.strip()
                message = message.strip()
                while message.endswith("\n"):
                    message[:-1]
                issues.append({
                    "path": path,
                    "lineno": lineno,
                    "type": type_,
                    "message": message,
                })
    with dest.open("w") as report_fp:
        ugly = etree.tostring(build_report(issues,
                                           path_prefix).getroot(), "utf-8")
        parsed = minidom.parseString(ugly)
        report_fp.write(parsed.toprettyxml(indent="  "))


def main(argv):
    logfile = None
    with open("Doxyfile", "r") as doxy_config:
        for line in doxy_config:
            match = RE_WARN_LOGFILE.match(line)
            if match:
                logfile = match.groups()[0].rstrip()
    if not logfile:
        print("No WARN_LOGFILE found in Doxyfile")
        return 1
    prefix = pathlib.Path(os.getcwd())
    logfile = pathlib.Path(logfile)
    log_to_junit(logfile, logfile.parent / "doxygen.xml", prefix)
    return 0


if __name__ == "__main__":
    main(sys.argv[1:])

