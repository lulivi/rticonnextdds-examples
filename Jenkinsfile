pipeline {
    agent {
        dockerfile true
    }

    environment {
        RTI_MIN_PACKAGE_URL = credentials('rti-min-package-url')
        RTI_PACKAGE_VERSION = credentials('rti-package-version')
        RTI_INSTALLATION_PATH = "${WORKSPACE}"
    }

    stages {
        stage('Download') {
            steps {
                sh 'python3 resources/ci_cd/linux_install.py'
            }
        }

        stage('Build') {
            steps {
                sh 'python3 resources/ci_cd/linux_build.py'
            }
        }

        stage('Static Analysis') {
            steps {
                sh 'python3 resources/ci_cd/linux_static_analysis.py'
            }
        }
    }
}
