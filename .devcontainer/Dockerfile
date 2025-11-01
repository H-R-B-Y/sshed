FROM ubuntu:24.04

# Install dependencies and notcurses from Ubuntu packages
RUN apt update && apt install -y \
	build-essential cmake git pkg-config \
	libtinfo-dev gdb \
	notcurses-bin libnotcurses-dev \
	locales \
	curl unzip \
	valgrind \
	bash-completion \
	&& apt clean

RUN apt install -y openssh-server

RUN echo "source /etc/bash_completion" >> /root/.bashrc

RUN apt install -y doxygen graphviz

RUN mkdir /var/run/sshd \
	&& mkdir -p /root/.ssh \
	&& chmod 700 /root/.ssh \
	&& ssh-keygen -A

RUN useradd -m -s /bin/bash gameuser \
	&& echo 'gameuser:gamepass' | chpasswd \
	&& mkdir -p /home/gameuser/.ssh \
	&& chmod 700 /home/gameuser/.ssh \
	&& chown gameuser:gameuser /home/gameuser/.ssh

# Create a minimal sshd_config
RUN echo "Port 2222" > /etc/ssh/sshd_config \
	&& echo "PasswordAuthentication yes" >> /etc/ssh/sshd_config \
	&& echo "PubkeyAuthentication no" >> /etc/ssh/sshd_config \
	&& echo "PermitRootLogin no" >> /etc/ssh/sshd_config \
	&& echo "UseDNS no" >> /etc/ssh/sshd_config \
	&& echo "LogLevel DEBUG" >> /etc/ssh/sshd_config \
	&& echo "AcceptEnv LANG LC_*" >> /etc/ssh/sshd_config

# Alternative: Use ForceCommand in sshd_config
RUN echo "Match User gameuser" >> /etc/ssh/sshd_config \
	&& echo "    ForceCommand /home/gameuser/sshed_client.out" >> /etc/ssh/sshd_config \
	&& echo "    PermitTTY yes" >> /etc/ssh/sshd_config \
	&& echo "    X11Forwarding no" >> /etc/ssh/sshd_config \
	&& echo "    AllowAgentForwarding no" >> /etc/ssh/sshd_config \
	&& echo "    AllowTcpForwarding no" >> /etc/ssh/sshd_config

# Configure locales 
RUN locale-gen en_US.UTF-8 \
	&& update-locale LANG=en_US.UTF-8 LANGUAGE=en_US:en LC_ALL=en_US.UTF-8 \
	&& echo "LANG=en_US.UTF-8" >> /etc/environment \
	&& echo "LANGUAGE=en_US:en" >> /etc/environment \
	&& echo "LC_ALL=en_US.UTF-8" >> /etc/environment \
	&& echo "export LANG=en_US.UTF-8" >> /home/gameuser/.bashrc \
	&& echo "export LANGUAGE=en_US:en" >> /home/gameuser/.bashrc \
	&& echo "export LC_ALL=en_US.UTF-8" >> /home/gameuser/.bashrc \
	&& chown gameuser:gameuser /home/gameuser/.bashrc

ENV LANG=en_US.UTF-8
ENV LANGUAGE=en_US:en
ENV LC_ALL=en_US.UTF-8

EXPOSE 2222

WORKDIR /workspace

# CMD ["/usr/sbin/sshd", "-D"]
