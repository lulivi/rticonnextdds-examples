pipeline {
    agent {
        dockerfile true
    }
    environment {
        RTI_MIN_PACKAGE_URL = credentials('rti-min-package-url')
        RTI_PACKAGE_VERSION = credentials('rti-package-version')
    }

    stages {
        stage('Download') {
            steps {
                sh 'pwd'
                sh 'cmake --version'
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