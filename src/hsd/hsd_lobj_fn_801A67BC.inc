    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r3, 0x10
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801A6858
    bge @L_801A6808
    cmpwi r3, 0x4
    beq @L_801A6848
    bge @L_801A67FC
    cmpwi r3, 0x2
    beq @L_801A6840
    bge @L_801A6880
    cmpwi r3, 0x1
    bge @L_801A6838
    b @L_801A6880
    @L_801A67FC:
    cmpwi r3, 0x8
    beq @L_801A6850
    b @L_801A6880
    @L_801A6808:
    cmpwi r3, 0x80
    beq @L_801A6870
    bge @L_801A682C
    cmpwi r3, 0x40
    beq @L_801A6868
    bge @L_801A6880
    cmpwi r3, 0x20
    beq @L_801A6860
    b @L_801A6880
    @L_801A682C:
    cmpwi r3, 0x100
    beq @L_801A6878
    b @L_801A6880
    @L_801A6838:
    li r31, 0x0
    b @L_801A6890
    @L_801A6840:
    li r31, 0x1
    b @L_801A6890
    @L_801A6848:
    li r31, 0x2
    b @L_801A6890
    @L_801A6850:
    li r31, 0x3
    b @L_801A6890
    @L_801A6858:
    li r31, 0x4
    b @L_801A6890
    @L_801A6860:
    li r31, 0x5
    b @L_801A6890
    @L_801A6868:
    li r31, 0x6
    b @L_801A6890
    @L_801A6870:
    li r31, 0x7
    b @L_801A6890
    @L_801A6878:
    li r31, 0x8
    b @L_801A6890
    @L_801A6880:
    la r3, lbl_8047DBB8(r2)
    li r4, 0x4a1
    la r5, lbl_8047DBD4(r2)
    bl fn_80196E10
    @L_801A6890:
    mr. r0, r31
    blt @L_801A68B8
    cmpwi r0, 0x9
    bge @L_801A68B8
    lis r3, lbl_804655E0@ha
    slwi r4, r0, 2
    addi r0, r3, lbl_804655E0@l
    add r3, r0, r4
    lwz r3, 0x0(r3)
    b @L_801A68BC
    @L_801A68B8:
    li r3, 0x0
    @L_801A68BC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
