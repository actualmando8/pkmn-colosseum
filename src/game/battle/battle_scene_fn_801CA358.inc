    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0xada
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA39C
    li r3, 0xa0f
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA394
    li r3, 0xb
    b @L_801CA4C0
@L_801CA394:
    li r3, 0xc
    b @L_801CA4C0
@L_801CA39C:
    li r3, 0xadb
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA3D4
    li r3, 0x483
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA3CC
    li r3, 0xb
    b @L_801CA4C0
@L_801CA3CC:
    li r3, 0xc
    b @L_801CA4C0
@L_801CA3D4:
    li r3, 0x42c
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA40C
    li r3, 0x42d
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA404
    li r3, 0x3
    b @L_801CA4C0
@L_801CA404:
    li r3, 0x4
    b @L_801CA4C0
@L_801CA40C:
    li r3, 0xad3
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA444
    li r3, 0xa08
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA43C
    li r3, 0x3
    b @L_801CA4C0
@L_801CA43C:
    li r3, 0x4
    b @L_801CA4C0
@L_801CA444:
    li r3, 0xad0
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA47C
    li r3, 0xa07
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA474
    li r3, 0x3
    b @L_801CA4C0
@L_801CA474:
    li r3, 0x4
    b @L_801CA4C0
@L_801CA47C:
    li r3, 0xacf
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CA4B4
    li r3, 0xa06
    bl fn_801902E0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_801CA4AC
    li r3, 0x1
    b @L_801CA4C0
@L_801CA4AC:
    li r3, 0x2
    b @L_801CA4C0
@L_801CA4B4:
    li r3, 0xa05
    bl fn_80190528
    li r3, 0x0
@L_801CA4C0:
    bl fn_80117AE4
    li r3, 0x1
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
