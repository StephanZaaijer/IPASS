.PHONY: clean All

All:
	@echo "----------Building project:[ Opdracht_1_3 -  ]----------"
	@cd "Opdracht_1_3" && "$(MAKE)" -f  "Opdracht_1_3.mk"
clean:
	@echo "----------Cleaning project:[ Opdracht_1_3 -  ]----------"
	@cd "Opdracht_1_3" && "$(MAKE)" -f  "Opdracht_1_3.mk" clean
