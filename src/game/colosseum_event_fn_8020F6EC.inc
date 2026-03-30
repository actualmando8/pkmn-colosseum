    nofralloc
    lis r3, 0x8038
    addi r3, r3, -24744
    addis r3, r3, 0x1
    lbz r0, 0x6002(r3)
    cmpwi r0, 0x2
    beq @L_8020F734
    bge @L_8020F718
    cmpwi r0, 0x0
    beq @L_8020F724
    bge @L_8020F72C
    b @L_8020F744
@L_8020F718:
    cmpwi r0, 0x4
    bge @L_8020F744
    b @L_8020F73C
@L_8020F724:
    li r3, 0x3
    blr
@L_8020F72C:
    li r3, 0x0
    blr
@L_8020F734:
    li r3, 0x1
    blr
@L_8020F73C:
    li r3, 0x2
    blr
@L_8020F744:
    li r3, 0x0
    blr
