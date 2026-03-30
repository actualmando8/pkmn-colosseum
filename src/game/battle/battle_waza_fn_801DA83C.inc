    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr. r29, r3
    beq @L_801DA8A8
    lbz r0, 0x74(r29)
    lwz r31, 0x68(r29)
    cmplwi r0, 0x0
    bne @L_801DA898
    b @L_801DA8A8
    b @L_801DA898
@L_801DA874:
    lbz r0, 0x14(r31)
    lwz r30, 0x34(r31)
    cmplwi r0, 0x0
    beq @L_801DA88C
    mr r3, r31
    bl fn_801DBB10
@L_801DA88C:
    mr r3, r31
    bl fn_801DBDDC
    mr r31, r30
@L_801DA898:
    cmplwi r31, 0x0
    bne @L_801DA874
    li r0, 0x0
    stw r0, 0x68(r29)
@L_801DA8A8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
