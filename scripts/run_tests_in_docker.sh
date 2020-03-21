docker image build -t tadahdtentityexperiment:latest .
docker container run  --interactive --env CODECOV_TOKEN=$CODECOV_TOKEN --tty --rm --name tadahdtentityexperiment tadahdtentityexperiment:latest

