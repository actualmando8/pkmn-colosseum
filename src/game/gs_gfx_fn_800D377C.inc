    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmpwi r3, 0x2
    stw r0, 0x14(r1)
    beq @L_800D37B0
    bge @L_800D37BC
    cmpwi r3, 0x1
    bge @L_800D37A0
    b @L_800D37BC
    @L_800D37A0:
    li r3, 0x0
    li r4, 0x0
    bl fn_8019C690
    b @L_800D37BC
    @L_800D37B0:
    li r3, 0x1
    li r4, 0x0
    bl fn_8019C690
    @L_800D37BC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
