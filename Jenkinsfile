pipeline {
    agent {
        dockerfile true
    }
    environment {
        RTI_MIN_PACKAGE_URL = credentials('rti-min-package-url')
        RTI_PACKAGE_VERSION = credentials('rti-package-version')
        RTI_INSTALLATION_PATH = $WORKSPACE
    }

    stages {
        stage('Download') {
            steps {
                sh 'pwd'
                sh 'cmake --version'
                sh 'echo $RTI_INSTALLATION_PATH'
                sh 'python resources/ci_cd/linux_install.py'
            }
        }

        stage('Build') {
            steps {
                sh 'python resources/ci_cd/linux_build.py'
            }
        }
    }
}