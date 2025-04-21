namespace nika::nas {

class FstabNAS final : public NAS {
public:
	std::vector<std::string> ListMountedDevices() const override;
};

}  // namespace nika::nas