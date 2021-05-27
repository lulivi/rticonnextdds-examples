#!/usr/bin/env python3
#
# (c) 2021 Copyright, Real-Time Innovations, Inc.  All rights reserved. RTI
# grants Licensee a license to use, modify, compile, and create derivative
# works of the Software.  Licensee has the right to distribute object form only
# for use with RTI products.  The Software is provided "as is", with no
# warranty of any type, including any warranty for fitness for any purpose. RTI
# is under no obligation to maintain or support the Software.  RTI shall not be
# liable for any incidental or consequential damages arising out of the use or
# inability to use the software.

"""
Creates a Markdown file with the output of the Jenkins stages
"""

import os
import sys
import platform
import subprocess
from pathlib import Path
from textwrap import dedent


def main():
    try:
        rti_installation_path = Path(
            os.getenv("RTI_INSTALLATION_PATH") or Path.home()
        ).resolve(strict=True)
    except FileNotFoundError:
        sys.exit("The RTI_INSTALLATION_PATH does not exist.")

    found_rti_connext_dds = list(
        rti_installation_path.glob("rti_connext_dds-?.?.?")
    )

    if not found_rti_connext_dds:
        rti_package_version = "-"
    else:
        rti_connext_dds_path = str(found_rti_connext_dds[0])
        rti_package_version = rti_connext_dds_path.split("-")[-1]

    text = f"""\
        This build is being executed on an internal Jenkins, only RTI
        employers can access the build logs. To give information to external
        users, we have configured the status checks.

        # Pipeline description
        The pipeline is defined
        [here](https://github.com/rticommunity/rticonnextdds-examples/blob/develop/Jenkinsfile)
        and the purpose of this pipeline is to build the examples using the
        last staging packages of RTI Connext® DDS and to analyze the build
        using `analyze-build` from `clang-tools`.

        # Environment details

        | Option                   | Setting               |
        | ------------------------ | --------------------- |
        | RTI Connext® DDS Version | {rti_package_version} |
        | System                   | {platform.system()}   |
        | Machine type             | {platform.machine()}  |
        | OS release               | {platform.release()}  |
    """

    text= dedent(text)

    if platform.system() == "Linux":
        gcc_version = (
            subprocess.run(["gcc", "-dumpfullversion"], capture_output=True)
            .stdout[:-1]
            .decode("utf-8")
        )
        text += f"| GCC Version              | {gcc_version}          |\n"

        clang_version = (
            subprocess.run(["clang", "-dumpversion"], capture_output=True)
            .stdout[:-1]
            .decode("utf-8")
        )
        text += f"| CLANG Version            | {clang_version}        |\n"
    elif platform.system() == "Darwin":
        output = subprocess.run(
            ["clang", "--version"], capture_output=True
        ).stdout.decode("utf-8")
        clang_version = output.split("\n")[0]

        text += f"| CLANG Version            | {clang_version}        |\n"


    with open("Dockerfile", "r") as file:
        text += "\n\n<details><summary>Dockerfile</summary>\n<p>\n"
        dockerfile = file.read()
        text += f"\n```Dockerfile\n{dockerfile}\n```\n"
        text += "</p>\n</details>"

    with open("Jenkinsfile", "r") as file:
        text += "\n<details><summary>Jenkinsfile</summary>\n<p>\n"
        jenkinsfile = file.read()
        text += f"\n```Groovy\n{jenkinsfile}\n```\n"
        text += "</p>\n</details>"

    with open("jenkins_output.md", "w") as file:
        file.write(text)


if __name__ == "__main__":
    main()
