BOX_NAME=CavewhereBox

.PHONY: vagrant clean
all: vagrant

vagrant:
	@vagrant up $(BOX_NAME)
	@vagrant ssh $(BOX_NAME) -- sudo /vagrant/setup.sh

clean:
	@vagrant destroy --force
