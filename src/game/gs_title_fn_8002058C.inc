    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x0
    li r3, -0x1
    stb r0, lbl_8047A32C(r13)
    bl fn_801EF644
    li r0, 0x1
    stb r0, lbl_8047A32C(r13)
    @L_800205B0:
    bl fn_800F0308
    b @L_800205B0
